"""
Web port of the original main.c "Evcil Hayvan Yonetim Sistemi".
Business rules (thresholds, deltas, the >=3-pet gate, mood classification
order/overlap) are ported 1:1 from the C source, including its original
quirks (e.g. the "Agliyor" branch is unreachable because the "Uzgun"
branch above it already catches every case that would satisfy it -
same as in main.c).

Everything below the ported core (time-based stat decay, achievements,
XP/levels, action history) is an additive layer built on top of it and
does not alter compute_mood / MOOD_EMOJI / ACTIONS in any way.
"""
from fastapi import FastAPI, Request, HTTPException
from fastapi.templating import Jinja2Templates
from fastapi.responses import HTMLResponse
from pydantic import BaseModel
from datetime import datetime
import uvicorn, os, time, json

app = FastAPI()
_dir = os.path.dirname(os.path.abspath(__file__))
templates = Jinja2Templates(directory=os.path.join(_dir, "templates"))

MIN_PETS_FOR_ACTIONS = 3

pets = [
    {"tur": "Kedi", "ad": "Pamuk", "enerji": 60.0, "mutluluk": 60.0,
     "xp": 0, "level": 1, "son_islem_zamani": time.time()},
    {"tur": "Kopek", "ad": "Karabas", "enerji": 60.0, "mutluluk": 60.0,
     "xp": 0, "level": 1, "son_islem_zamani": time.time()},
    {"tur": "Kus", "ad": "Sari", "enerji": 60.0, "mutluluk": 60.0,
     "xp": 0, "level": 1, "son_islem_zamani": time.time()},
]


def compute_mood(enerji, mutluluk, previous):
    # Same if/elif order as hayvanlar_duygu_durumu() in main.c.
    if enerji > 25 and mutluluk > 50:
        return "Mutlu"
    elif enerji < 20 and mutluluk < 20:
        return "Uzgun"
    elif enerji < 10 and mutluluk < 10:
        return "Agliyor"
    elif enerji < 5:
        return "Uyuyor"
    elif enerji < 20:
        return "Ac"
    elif enerji > 40 or mutluluk < 30:
        return "Oyun Istiyor"
    return previous  # no branch matched -> mood unchanged, as in the C code


MOOD_EMOJI = {
    "Mutlu": "😄", "Uzgun": "😢", "Agliyor": "😭",
    "Uyuyor": "😴", "Ac": "🍖", "Oyun Istiyor": "🎮",
}

# Presentational-only maps (CSS classes / icons). Do not affect mood logic.
MOOD_CLASS = {
    "Mutlu": "mutlu", "Uzgun": "uzgun", "Agliyor": "agliyor",
    "Uyuyor": "uyuyor", "Ac": "ac", "Oyun Istiyor": "oyun",
}
ACTION_ICONS = {
    "yemek": "🍗", "oyun": "🎾", "temizle": "🧼", "sevgi": "💗",
    "uyut": "🛌", "odul": "🏆", "su": "💧",
}

ACTIONS = {
    # key: (label, enerji_delta, mutluluk_delta, check_enerji, check_mutluluk)
    "yemek": ("Yemek Ver", 15, 10, True, True),
    "oyun": ("Oyun Oynat", -15, 15, True, True),
    "temizle": ("Temizle", -10, -5, True, True),
    "sevgi": ("Sevgi Goster", 0, 5, True, True),
    "uyut": ("Uyut", 5, 0, False, True),
    "odul": ("Odul Ver", 0, 10, True, True),
    "su": ("Su Ver", 5, 5, True, True),
}

# ---------------------------------------------------------------------------
# Additive layer: time-based decay, XP/levels, achievements, action history.
# None of this touches compute_mood/MOOD_EMOJI/ACTIONS above; it only calls
# compute_mood() to recompute mood after stats change.
# ---------------------------------------------------------------------------

# Real-time stat decay rates (points per real-world minute). Kept separate
# per stat so pets feel like they get hungrier faster than they get bored.
DECAY_PER_MINUTE_ENERJI = 5.0
DECAY_PER_MINUTE_MUTLULUK = 3.0

XP_PER_ACTION = 10
LEVEL_XP_BASE = 100
LEVEL_XP_STEP = 50

HISTORY_LIMIT = 200
history = []          # list of {zaman, ad, aksiyon, mesaj}
total_action_count = 0
unlocked_achievements = set()

ACHIEVEMENTS = [
    {"id": "ilk_adim", "ad": "Ilk Adim", "emoji": "🐾",
     "aciklama": "Ilk kez bir evcil hayvana baktin."},
    {"id": "bakici", "ad": "5 Gun Bakim", "emoji": "🏅",
     "aciklama": "Toplam 25 bakim aksiyonu tamamladin."},
    {"id": "mutlu_aile", "ad": "Mutlu Aile", "emoji": "🎉",
     "aciklama": "Tum evcil hayvanlarin ayni anda Mutlu oldu."},
    {"id": "yildiz_bakici", "ad": "Yildiz Bakici", "emoji": "⭐",
     "aciklama": "Bir evcil hayvan 5. seviyeye ulasti."},
    {"id": "kalabalik_ev", "ad": "Kalabalik Ev", "emoji": "🏠",
     "aciklama": "Ayni anda 5 veya daha fazla evcil hayvana sahip oldun."},
]


def decay_pet(p, now=None):
    """Apply real-elapsed-time stat decay to a single pet, then recompute
    its mood by calling the untouched compute_mood(). Idempotent: calling
    it twice back-to-back applies ~0 extra decay the second time."""
    now = now if now is not None else time.time()
    last = p.get("son_islem_zamani", now)
    elapsed_min = max(0.0, (now - last) / 60.0)
    if elapsed_min > 0:
        p["enerji"] = max(0.0, p["enerji"] - elapsed_min * DECAY_PER_MINUTE_ENERJI)
        p["mutluluk"] = max(0.0, p["mutluluk"] - elapsed_min * DECAY_PER_MINUTE_MUTLULUK)
        p["duygu_durumu"] = compute_mood(p["enerji"], p["mutluluk"], p.get("duygu_durumu", "Mutlu"))
    p["son_islem_zamani"] = now


def decay_all_pets():
    now = time.time()
    for p in pets:
        decay_pet(p, now)


def next_level_xp(level):
    return LEVEL_XP_BASE + (level - 1) * LEVEL_XP_STEP


def gain_xp(p, amount=XP_PER_ACTION):
    p["xp"] = p.get("xp", 0) + amount
    leveled_up = False
    while p["xp"] >= next_level_xp(p.get("level", 1)):
        p["xp"] -= next_level_xp(p.get("level", 1))
        p["level"] = p.get("level", 1) + 1
        leveled_up = True
    return leveled_up


def log_history(ad, aksiyon, mesaj):
    history.append({"zaman": time.time(), "ad": ad, "aksiyon": aksiyon, "mesaj": mesaj})
    if len(history) > HISTORY_LIMIT:
        del history[: len(history) - HISTORY_LIMIT]


def check_achievements():
    if total_action_count >= 1:
        unlocked_achievements.add("ilk_adim")
    if total_action_count >= 25:
        unlocked_achievements.add("bakici")
    if len(pets) >= MIN_PETS_FOR_ACTIONS and pets and all(p["duygu_durumu"] == "Mutlu" for p in pets):
        unlocked_achievements.add("mutlu_aile")
    if any(p.get("level", 1) >= 5 for p in pets):
        unlocked_achievements.add("yildiz_bakici")
    if len(pets) >= 5:
        unlocked_achievements.add("kalabalik_ev")


def find_pet(ad):
    for p in pets:
        if p["ad"] == ad:
            return p
    raise HTTPException(404, "Evcil hayvan bulunamadi")


def serialize(p):
    mood = p.get("duygu_durumu", "Mutlu")
    level = p.get("level", 1)
    return {
        "tur": p["tur"],
        "ad": p["ad"],
        "enerji": round(max(0.0, min(100.0, p["enerji"])), 1),
        "mutluluk": round(max(0.0, min(100.0, p["mutluluk"])), 1),
        "duygu_durumu": mood,
        "emoji": MOOD_EMOJI.get(mood, "🐾"),
        "mood_class": MOOD_CLASS.get(mood, "mutlu"),
        "xp": p.get("xp", 0),
        "level": level,
        "next_level_xp": next_level_xp(level),
    }


for p in pets:
    p["duygu_durumu"] = compute_mood(p["enerji"], p["mutluluk"], "Mutlu")


class NewPet(BaseModel):
    tur: str
    ad: str


class RenamePet(BaseModel):
    yeni_ad: str


@app.get("/", response_class=HTMLResponse)
async def index(request: Request):
    decay_all_pets()
    check_achievements()
    actions_list = [{"key": k, "label": v[0], "icon": ACTION_ICONS.get(k, "🐾")} for k, v in ACTIONS.items()]
    achievements_list = [{**a, "kazanildi": a["id"] in unlocked_achievements} for a in ACHIEVEMENTS]
    history_list = [
        {**h, "zaman_str": datetime.fromtimestamp(h["zaman"]).strftime("%H:%M:%S")}
        for h in reversed(history[-20:])
    ]
    return templates.TemplateResponse("index.html", {
        "request": request,
        "pets": [serialize(p) for p in pets],
        "min_pets": MIN_PETS_FOR_ACTIONS,
        "actions_list": actions_list,
        "actions_json": json.dumps(actions_list, ensure_ascii=False),
        "unlocked": len(pets) >= MIN_PETS_FOR_ACTIONS,
        "achievements": achievements_list,
        "achievements_json": json.dumps(achievements_list, ensure_ascii=False),
        "history": history_list,
    })


@app.get("/api/pets")
async def list_pets():
    decay_all_pets()
    check_achievements()
    return {"pets": [serialize(p) for p in pets], "unlocked": len(pets) >= MIN_PETS_FOR_ACTIONS}


@app.post("/api/pets")
async def add_pet(data: NewPet):
    if any(p["ad"] == data.ad for p in pets):
        raise HTTPException(400, "Bu isimde bir evcil hayvan zaten var")
    now = time.time()
    p = {"tur": data.tur, "ad": data.ad, "enerji": 60.0, "mutluluk": 60.0,
         "xp": 0, "level": 1, "son_islem_zamani": now}
    p["duygu_durumu"] = compute_mood(60.0, 60.0, "Mutlu")
    pets.append(p)
    log_history(p["ad"], "eklendi", f"{p['ad']} aileye katildi")
    check_achievements()
    return {"pets": [serialize(p) for p in pets], "unlocked": len(pets) >= MIN_PETS_FOR_ACTIONS}


@app.put("/api/pets/{ad}")
async def rename_pet(ad: str, data: RenamePet):
    p = find_pet(ad)
    if any(x["ad"] == data.yeni_ad for x in pets):
        raise HTTPException(400, "Bu isimde bir evcil hayvan zaten var")
    eski_ad = p["ad"]
    p["ad"] = data.yeni_ad
    log_history(data.yeni_ad, "yeniden_adlandirildi", f"{eski_ad} -> {data.yeni_ad}")
    return {"pets": [serialize(p) for p in pets]}


@app.delete("/api/pets/{ad}")
async def delete_pet(ad: str):
    find_pet(ad)
    pets[:] = [p for p in pets if p["ad"] != ad]
    log_history(ad, "silindi", f"{ad} ailenden ayrildi")
    return {"pets": [serialize(p) for p in pets], "unlocked": len(pets) >= MIN_PETS_FOR_ACTIONS}


@app.post("/api/pets/{ad}/{action}")
async def do_action(ad: str, action: str):
    global total_action_count
    if action not in ACTIONS:
        raise HTTPException(404, "Bilinmeyen aksiyon")
    if len(pets) < MIN_PETS_FOR_ACTIONS:
        raise HTTPException(400, f"Hayvan sayisi {MIN_PETS_FOR_ACTIONS} veya daha fazla olmalidir!")
    p = find_pet(ad)
    decay_pet(p)
    label, de, dm, check_e, check_m = ACTIONS[action]
    if check_e and p["enerji"] < 5:
        raise HTTPException(400, "Evcil hayvanin enerji seviyesi dusuk. Uyutmaniz gerekiyor.")
    if check_m and p["mutluluk"] < 5:
        raise HTTPException(400, "Evcil hayvanin mutluluk seviyesi dusuk. Eglendirmeniz gerekiyor.")
    p["enerji"] += de
    p["mutluluk"] += dm
    p["duygu_durumu"] = compute_mood(p["enerji"], p["mutluluk"], p["duygu_durumu"])
    leveled_up = gain_xp(p)
    total_action_count += 1
    message = f"{label}: {p['ad']}"
    if leveled_up:
        message += f" | Seviye atladi -> {p['level']}"
    log_history(p["ad"], action, message)
    check_achievements()
    return {"pet": serialize(p), "message": message, "leveled_up": leveled_up}


@app.get("/api/achievements")
async def list_achievements():
    return {"achievements": [
        {**a, "kazanildi": a["id"] in unlocked_achievements} for a in ACHIEVEMENTS
    ]}


@app.get("/api/history")
async def list_history(limit: int = 20):
    limit = max(1, min(limit, HISTORY_LIMIT))
    items = list(reversed(history[-limit:]))
    return {"history": [
        {**h, "zaman_str": datetime.fromtimestamp(h["zaman"]).strftime("%H:%M:%S")}
        for h in items
    ]}


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=5013)
