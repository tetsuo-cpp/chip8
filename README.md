# CHIP-8 Emulator
This project is a unit tested C++ implementation of the CHIP-8 virtual machine.

The emulator can be used to play game ROMs designed for the CHIP-8 system.
## Dependencies
* SDL2
* Google Test
* Google Mock
## Usage
The emulator requires SDL2 which can be installed like so (or the equivalent for your package manager):
```
sudo yum install SDL2-devel
```
The Google Test and Google Mock frameworks recommend that you do not install and link against them as pre-compiled libraries.
Therefore, you will need to get the sources for both and edit the appropriate directory variables in the Makefile.
### Emulator
```
make chip8
./chip8 [filename]
```
### Unit Tests
```
make chip8_test
./chip8_test
```
