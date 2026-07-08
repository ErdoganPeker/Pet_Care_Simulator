from fastapi import FastAPI, Request
from fastapi.templating import Jinja2Templates
from fastapi.responses import HTMLResponse
import uvicorn, time

app = FastAPI()
templates = Jinja2Templates(directory="templates")

pet = {"name":"Fluffy","type":"cat","hunger":70,"happiness":65,"cleanliness":80,"energy":75}
last = [time.time()]

def decay():
    elapsed = min(time.time()-last[0], 300)
    r = elapsed/60
    pet["hunger"]=max(0,pet["hunger"]-r*5)
    pet["happiness"]=max(0,pet["happiness"]-r*3)
    pet["cleanliness"]=max(0,pet["cleanliness"]-r*4)
    pet["energy"]=max(0,pet["energy"]-r*2)
    last[0]=time.time()

def status():
    avg=(pet["hunger"]+pet["happiness"]+pet["cleanliness"]+pet["energy"])/4
    if avg>=80: return "Excellent!","😄"
    if avg>=60: return "Good","🙂"
    if avg>=40: return "Okay","😐"
    return "Needs Care!","😟"

@app.get("/", response_class=HTMLResponse)
async def index(request: Request):
    decay()
    st,em=status()
    return templates.TemplateResponse("index.html", {"request":request,"pet":pet,"status":st,"emoji":em})

@app.post("/action/{action}")
async def action(action: str):
    decay()
    if action=="feed": pet["hunger"]=min(100,pet["hunger"]+30)
    elif action=="play": pet["happiness"]=min(100,pet["happiness"]+25);pet["energy"]=max(0,pet["energy"]-15)
    elif action=="clean": pet["cleanliness"]=min(100,pet["cleanliness"]+35)
    elif action=="sleep": pet["energy"]=min(100,pet["energy"]+40)
    st,em=status()
    return {"pet":pet,"status":st,"emoji":em}

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=5013)
