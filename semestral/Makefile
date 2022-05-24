CXX = g++
LD = g++
CXXFLAGS = -std=c++17 -Wall -pedantic -Wno-long-long -O2
CXXFLAGS_DEBUG = $(CXXFLAGS) -D PN_DEBUG -g -pg -fPIE
LIBS = -lncurses # -lstdc++fs

# echo $(find src | grep '.*\.h$' | sed 's/^.*\///' | sed 's/.h/.o/' | sort -f | tr '\n' ' ')
OBJECTS = Ant.o AntLine.o AntNest.o Application.o ComputerPlayer.o Dialog.o Game.o GameObject.o GameScreen.o log.o Player.o PlayerUnit.o Save.o SaveList.o Screen.o Wall.o
OBJECTS_MAIN = main.o $(OBJECTS)
OBJECTS_TEST = test.o $(OBJECTS)

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
	rm -rf $(TARGET) $(TARGET_TEST) $(TARGET).zip log Makefile.d $(BUILD_DIR) doc 

doc: $(SRC_DIR)/* $(SRC_DIR)/**/*
	doxygen Doxyfile

submit: $(TARGET).zip

lc:
	find $(SRC_DIR) -type f | sort -f | xargs wc -l

$(TARGET).zip: clean
	rm -rf /tmp/$(TARGET)
	cp -r . /tmp/$(TARGET)
	cd /tmp && \
	zip -rmT $(shell pwd)/$(TARGET).zip $(TARGET)

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

Makefile.d: $(SRC_DIR)/*.cpp $(SRC_DIR)/**/*.cpp
	$(CXX) -MM $(SRC_DIR)/*.cpp $(SRC_DIR)/**/*.cpp > Makefile.d

.PHONY: all clean compile compile_debug debug lc log run submit test

include Makefile.d
