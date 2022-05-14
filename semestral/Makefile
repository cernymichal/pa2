CXX = g++
LD = g++
CXXFLAGS = -std=c++17 -Wall -pedantic -Wno-long-long -O2
CXXFLAGS_DEBUG = $(CXXFLAGS) -D PN_DEBUG -g -pg -fPIE
LIBS = -lncurses

# echo $(find src | grep '.*\.cpp$' | grep -v '.*\.t.cpp' | grep -v -E '^src/(main|test)\.cpp$' | sed 's/^.*\///' | sed 's/.cpp/.o/' | sort -f | tr '\n' ' ')
OBJECTS = Ant.o AntNest.o ComputerPlayer.o Dialog.o Game.o GameObject.o Label.o log.o Player.o Program.o Screen.o Wall.o
OBJECTS_MAIN = $(OBJECTS) main.o
OBJECTS_TEST = $(OBJECTS) test.o

TARGET = cernym65
TARGET_TEST = $(TARGET)_test
SRC_DIR = src
BUILD_DIR = build

vpath %.o $(BUILD_DIR)

all: compile doc

compile: $(TARGET)

compile_debug: CXXFLAGS := $(CXXFLAGS_DEBUG)
compile_debug: compile

run: compile
	./$(TARGET)

debug: compile_debug
	./$(TARGET)

log:
	less +F log

test: CXXFLAGS := $(CXXFLAGS_DEBUG)
test: $(TARGET_TEST)
	./$(TARGET_TEST)

clean:
	rm -rf $(TARGET) $(TARGET_TEST) $(BUILD_DIR) log doc Makefile.d

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

%.o: $(SRC_DIR)/**/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $(BUILD_DIR)/$@ $<

Makefile.d:
	$(CXX) -MM $(SRC_DIR)/*.cpp $(SRC_DIR)/**/*.cpp > Makefile.d

.PHONY: all clean debug compile compile_debug log run test

include Makefile.d
