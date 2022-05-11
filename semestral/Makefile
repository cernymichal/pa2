CXX = g++
LD = g++
CXXFLAGS = -std=c++17 -Wall -pedantic -Wno-long-long -O2
LIBS = -lncurses

OBJECTS = Ant.o AntNest.o ComputerPlayer.o Game.o GameObject.o Label.o Player.o Program.o Wall.o
OBJECTS_MAIN = main.o $(OBJECTS)
OBJECTS_TEST = test.o $(OBJECTS)

TARGET = cernym65
TARGET_TEST = $(TARGET)_test
SRC_DIR = src
BUILD_DIR = build

vpath %.o $(BUILD_DIR)

all: compile doc

compile: $(TARGET)

compile_debug: CXXFLAGS := -D PN_DEBUG -g -pg -fPIE $(CXXFLAGS)
compile_debug: compile

run: compile
	./$(TARGET)

debug: compile_debug
	./$(TARGET)

test: $(TARGET_TEST)
	./$(TARGET_TEST)

clean:
	rm -rf $(TARGET) $(TARGET_TEST) $(BUILD_DIR) doc Makefile.d

doc: src/*
	doxygen Doxyfile

$(TARGET): $(OBJECTS_MAIN)
	cd $(BUILD_DIR) && \
	$(LD) -o ../$(TARGET) $(OBJECTS_MAIN) $(LIBS)

$(TARGET_TEST): $(OBJECTS_TEST)
	cd $(BUILD_DIR) && \
	$(LD) -o ../$(TARGET_TEST) $(OBJECTS_TEST) $(LIBS)

%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $(BUILD_DIR)/$@ $<

Makefile.d:
	$(CXX) -MM $(SRC_DIR)/*.cpp > Makefile.d

.PHONY: all clean compile run test

include Makefile.d
