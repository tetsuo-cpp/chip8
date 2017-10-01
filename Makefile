CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g $(shell root-config --cflags) -Ilib
LDFLAGS=-g $(shell root-config --ldflags) -lSDL2
LDLIBS=-g $(shell root-config --libs)

SRCS= \
	src/main.cc \
	lib/chip8_lib/chip8.cc \
	lib/chip8_lib/cpu.cc \
	lib/chip8_lib/rom.cc \
	lib/chip8_lib/display.cc \
	lib/chip8_lib/controller.cc \
	lib/chip8_lib/random.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: chip8

chip8: $(OBJS)
	$(CXX) $(LDFLAGS) -o chip8 $(OBJS) $(LDLIBS)

main.o: \
	src/main.cc \
	chip8_lib/chip8.h

chip8.o: \
	chip8_lib/chip8.h \
	chip8_lib/chip8.cc \
	chip8_lib/cpu.h \
	chip8_lib/rom.h \
	chip8_lib/display.h \
	chip8_lib/controller.h

cpu.o: \
	chip8_lib/cpu.h \
	chip8_lib/cpu.cc \
	chip8_lib/interfaces.h

rom.o: \
	chip8_lib/rom.h \
	chip8_lib/rom.cc \
	chip8_lib/interfaces.h

display.o: \
	chip8_lib/display.h \
	chip8_lib/display.cc \
	chip8_lib/interfaces.h

controller.o: \
	chip8_lib/controller.h \
	chip8_lib/controller.cc \
	chip8_lib/interfaces.h

random.o: \
	chip8_lib/random.h \
	chip8_lib/random.cc \
	chip8_lib/interfaces.h

clean:
	$(RM) $(OBJS)
