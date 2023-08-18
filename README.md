# SAMobile
An experimental MTA and SAMP implementation for GTA SA v210 on AArch64

## Project aims
- Complete replacement for build69 (SAMP for ARMv7 from GTASA v1.8 what has deprecated)

## Screenshots (work-in-progress), thorough testing, and additional fixes are still needed

<img src="screens/Screenshot0.png" alt="Screenshot 0" style="width:50%;">
<img src="screens/Screenshot1.png" alt="Screenshot 1" style="width:50%;">

> Most notable things: 1. ImGUI is almost working properly as expected - 2. We can draw things on the surface

## How to compile?
- Before start make sure that you have configured the `env.json` file in the project folder

## Limitations of hook system
- Can't to replace or emplace instructions that are a 'ret' or any type of branch
- Can't to replace or save any instruction that changes or modifies the x8 or x30 registers (only the function prologue is allowed)

## Todo
```
- [x] To load our 3 (three) texture databases at startup
- [x] To create two database directories that utilize the game's texture database system, providing users with the ability to load or replace game objects and textures
- [x] To hook class methods using the jump-to-trampoline technique
- [x] To fix CEntity_UpdateRpHAnim hooking and restores from LP, making both LP and PC versions work
- [x] Display the FPS at the bottom of the screen
```
