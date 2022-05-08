CXX = g++
LD = g++
CXXFLAGS = -std=c++17 -Wall -pedantic -Wno-long-long -O2
LIBS = -lncurses
OBJECTS = main.o Ant.o AntNest.o ComputerPlayer.o Game.o GameObject.o Label.o Player.o Program.o Wall.o

LOGIN = cernym65
SRC_DIR = src
BUILD_DIR = build

vpath %.o $(BUILD_DIR)

all: compile doc

compile: $(LOGIN)

run: $(LOGIN)
	./$(LOGIN)

clean:
	rm -rf $(LOGIN) $(BUILD_DIR) doc Makefile.d

doc: src/*
	doxygen Doxyfile

$(LOGIN): $(OBJECTS)
	cd $(BUILD_DIR) && \
	$(LD) -o ../$(LOGIN) $(OBJECTS) $(LIBS)

%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $(BUILD_DIR)/$@ $<

Makefile.d:
	$(CXX) -MM $(SRC_DIR)/*.cpp > Makefile.d

.PHONY: all clean compile run

include Makefile.d
