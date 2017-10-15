CC=gcc
CXX=g++
RM=rm -f
GTEST_DIR=${HOME}/lib/googletest-release-1.8.0/googletest
GMOCK_DIR=${HOME}/lib/googletest-release-1.8.0/googlemock
CPPFLAGS=-g $(shell root-config --cflags) -Wall -Wextra -Werror -Ilib -I$(GTEST_DIR)/include -I$(GMOCK_DIR)/include
LDFLAGS=-g $(shell root-config --ldflags) -lSDL2
LDLIBS=-g $(shell root-config --libs)

GTEST_HEADERS= \
	$(GTEST_DIR)/include/gtest/*.h \
	$(GTEST_DIR)/include/gtest/internal/*.h

GMOCK_HEADERS= \
	$(GMOCK_DIR)/include/gmock/*.h \
	$(GMOCK_DIR)/include/gmock/internal/*.h \
	$(GTEST_HEADERS)

GTEST_SRCS=$(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GMOCK_SRCS=$(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)

SRCS= \
	src/main.cc \
	lib/chip8_lib/chip8.cc \
	lib/chip8_lib/cpu.cc \
	lib/chip8_lib/rom.cc \
	lib/chip8_lib/display.cc \
	lib/chip8_lib/controller.cc \
	lib/chip8_lib/clock.cc

OBJS=$(subst .cc,.o,$(SRCS))

TEST_SRCS= \
	test/test_main.cc \
	test/cpu_test.cc \
	lib/chip8_lib/cpu.cc

TEST_OBJS=$(subst .cc,.o,$(TEST_SRCS))

all: chip8 chip8_test

chip8: $(OBJS)
	$(CXX) $(LDFLAGS) -o chip8 $(OBJS) $(LDLIBS)

chip8_test: $(TEST_OBJS) gmock.a
	$(CXX) $(LDFLAGS) -o chip8_test $(TEST_OBJS) gmock.a $(LDLIBS)

main.o: \
	src/main.cc \
	chip8_lib/chip8.h

chip8.o: \
	chip8_lib/chip8.h \
	chip8_lib/chip8.cc \
	chip8_lib/cpu.h \
	chip8_lib/rom.h \
	chip8_lib/display.h \
	chip8_lib/controller.h \
	chip8_lib/random.h \
	chip8_lib/clock.h

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

clock.o: \
	chip8_lib/clock.h \
	chip8_lib/clock.cc \
	chip8_lib/interfaces.h

test_main.o: \
	test/test_main.cc \
	test/cpu_test.cc

cpu_test.o: \
	test/cpu_test.cc \
	chip8_test_lib/test_cpu.h \
	chip8_lib/cpu.h \
	chip8_lib/cpu.cc \
	chip8_test_lib/mocks.h

gtest-all.o: $(GTEST_SRCS)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(LDFLAGS) \
		-c $(GTEST_DIR)/src/gtest-all.cc

gmock-all.o: $(GMOCK_SRCS)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(LDFLAGS) \
		-c $(GMOCK_DIR)/src/gmock-all.cc

gmock.a: gmock-all.o gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) $(OBJS) $(TEST_OBJS)
