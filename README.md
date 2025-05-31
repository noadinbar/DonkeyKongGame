# Donkey Kong Game

A modern C++17 console-based remake of the classic **Donkey Kong** game.  
In this game, you play as **Mario (`@`)**, who must rescue **Pauline (`$`)** while avoiding barrels and ghosts.  
You can pick up a **hammer (`p`)** to destroy enemies, score points, and survive longer.  
The game includes three levels, a life system, and two gameplay modes: manual and automatic.

> Best viewed in an **80x25** terminal window.

---

# Game Features

- Three levels with increasing difficulty.
- Manual mode: play the game in real time using keyboard input.
- Automatic mode: replays the last recorded manual session.
- 3 lives per game.
- Score system that rewards collecting items and defeating enemies.
- Obstacles include barrels and ghosts.
- You can pick up a hammer (`p`) to kill enemies.
- All steps and outcomes in manual mode are saved and reused in automatic mode.

---

# Controls

To move Mario (only on english keyboard):
  - a : Move left
  - d : Move right
  - s : Stay in place
  - w : Climb up a ladder
  - x : Climb down a ladder

To use a hammer:
  - p : Use hammer (only when collected)

To exit\pause the game at any time:
  - ESC

> Mario is represented by `@`  
> Pauline is represented by `$`  
> The hammer is represented by `p`

---

# Technologies

- Language: **C++17**
- Platform: **Windows** (Console)
- Recommended IDE: **Visual Studio 2019 or newer**
- No external libraries required

---

# How to Run:

Recommended: Using Visual Studio

1. Open the project in Visual Studio.
2. Make sure your project is set to use the **C++17 standard**:
   - Project Properties → C/C++ → Language → C++ Language Standard → `ISO C++17`
3. Add all `.cpp` and `.h` files to your project if needed.
4. Build and run.

> This method ensures full compatibility with C++17 features like `<filesystem>`.

**Not recommended: Using CMD with g++**

To compile with g++, you **must** have version **10 or higher** with full C++17 support.  
Older versions (like 6.3) will not work due to missing `<filesystem>` support.

---

# Screenshot
![Game Screenshot](screenshot.png)

---

# License

This project is not licensed for public use.
You may not copy, modify, distribute, or use this code without explicit permission from the author.

---
