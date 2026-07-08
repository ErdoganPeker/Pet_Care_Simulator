# Pet Care Simulator

![C](https://img.shields.io/badge/C-99-A8B9CC?style=flat&logo=c&logoColor=white)
![Game Logic](https://img.shields.io/badge/Type-Virtual%20Pet%20Simulator-ff6b6b?style=flat)
![Algorithm](https://img.shields.io/badge/Algorithm-State%20Machine-8b5cf6?style=flat)

> A terminal-based virtual pet simulator written in C — keep your pet happy, healthy, and entertained by feeding, cleaning, and fulfilling its needs before they run out.

---

## Overview

The player adopts a virtual pet whose well-being is tracked across multiple attributes — hunger, cleanliness, happiness, and energy. Each attribute decays over time. The player must respond to the pet's needs through menu-driven actions before any stat hits critical levels. The simulation runs on a loop with a simple but engaging state-machine game logic.

## Features

- Virtual pet with multiple tracked stats: hunger, cleanliness, happiness, energy
- Stats decay over time — neglect leads to a sad or unhealthy pet
- Actions: feed, clean, play, put to sleep, check status
- Persistent pet data saved to file between sessions (`evcil_veri.txt`)
- Visual status output showing each stat as a level
- Sample output files included for testing (`a_cikti.txt`, `b_cikti.txt`, etc.)
- Emotion/mood system that reflects the overall state of the pet (`duygu.txt`)

## Tech Stack

- **Language:** C (C99 standard)
- **Storage:** File I/O for session persistence
- **Logic:** State machine with timed attribute decay
- **I/O:** Terminal-based interactive menu

## Project Structure

```
Pet_Care_Simulator/
├── main.c              # Core simulator — game loop, state machine, menus
├── evcil_veri.txt      # Saved pet stats (auto-generated)
├── duygu.txt           # Pet mood/emotion log
├── a_cikti.txt         # Sample test output A
├── aaa_cikti.txt       # Sample test output AAA
├── b_cikti.txt         # Sample test output B
├── c_cikti.txt         # Sample test output C
├── e_cikti.txt         # Sample test output E
└── .gitignore
```

## Getting Started

### Prerequisites

- GCC or any C99-compatible compiler

### Build & Run

```bash
git clone https://github.com/ErdoganPeker/Pet_Care_Simulator.git
cd Pet_Care_Simulator
gcc -std=c99 -o pet_sim main.c
./pet_sim
```

On Windows with MinGW:

```bash
gcc -std=c99 -o pet_sim.exe main.c
pet_sim.exe
```

## Gameplay

```
Your pet's current status:
  Hunger    : ████████░░  80%
  Cleanliness: ██████░░░░  60%
  Happiness : ███░░░░░░░  30%  <-- your pet is unhappy!
  Energy    : █████████░  90%

What would you like to do?
  1. Feed
  2. Clean
  3. Play
  4. Let it sleep
  5. Check status
  0. Save & Exit
```

Pet stats are saved automatically on exit and restored on next launch.

## Author

**Erdogan Yasin Peker** — Computer Engineer

[GitHub](https://github.com/ErdoganPeker) · [LinkedIn](https://www.linkedin.com/in/erdogan-yasin-peker-b107ba24b/)
