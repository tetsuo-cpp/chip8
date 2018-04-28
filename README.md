# CHIP-8 Emulator
This emulator is a unit tested C++ implementation of the CHIP-8 virtual machine.

It can play game ROMs designed for the CHIP-8 system!
## Screenshots
### Invaders
![Invaders](invaders.png)
### Tetris
![Tetris](tetris.png)
## Dependencies
* CMake
* SDL2
* Google Test
* Google Mock
## Usage
The emulator requires SDL2 which can be installed via your package manager.

The unit tests require Google Test and Google Mock. Both of these frameworks recommend that you do not install and link against them as pre-compiled libraries. Therefore, this project's CMake setup will automatically clone the sources for these frameworks and compile them as part of the build.
```
cmake --build .
```
### Emulator
```
./chip8 [filename]
```
### Unit Tests
```
./chip8_test
```
