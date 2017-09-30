CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags) -lSDL2
LDLIBS=-g $(shell root-config --libs)

SRCS=src/main.cc lib/chip8.cc lib/cpu.cc lib/rom.cc lib/display.cc lib/controller.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: chip8

chip8: $(OBJS)
	$(CXX) $(LDFLAGS) -o chip8 $(OBJS) $(LDLIBS)

main.o: src/main.cc lib/chip8.h

chip8.o: lib/chip8.h lib/chip8.cc lib/cpu.h lib/rom.h lib/display.h lib/controller.h

cpu.o: lib/cpu.h lib/cpu.cc lib/rom.h lib/display.h lib/controller.h

rom.o: lib/rom.h lib/rom.cc

display.o: lib/display.h lib/display.cc

controller.o: lib/controller.h lib/controller.cc

clean:
	$(RM) $(OBJS)
