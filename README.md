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

The unit tests require Google Test and Google Mock. Both of these frameworks recommend that you do not install and link against them as pre-compiled libraries. Therefore, these frameworks have been added as Git submodules and are compiled as part of the CMake build.
```
git submodule init
git submodule update
cmake .
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
