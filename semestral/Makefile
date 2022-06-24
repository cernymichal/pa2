CXX = g++
LD = g++
CXXFLAGS = -std=c++17 -Wall -pedantic -Wno-long-long -O2
CXXFLAGS_DEBUG = $(CXXFLAGS) -D PN_DEBUG -g -pg -fPIE
LIBS = -lncurses -lstdc++fs

OBJECTS = $(shell find src | grep '.*\.h$$' | sed 's/^.*\///' | sed 's/\.h$$/.o/' | tr '\n' ' ')
OBJECTS_MAIN = main.o $(OBJECTS)
OBJECTS_TEST = test.o $(OBJECTS)

TARGET = PinsAndNeedles
TARGET_TEST = $(TARGET)_test
SRC_DIR = src
BUILD_DIR = build

vpath %.o $(BUILD_DIR)

all: compile doc

compile: $(TARGET)

compile_debug: CXXFLAGS := $(CXXFLAGS_DEBUG)
compile_debug: compile

run: compile
	"./$(TARGET)"

debug: compile_debug
	"./$(TARGET)"

log:
	less +F log

test: CXXFLAGS := $(CXXFLAGS_DEBUG)
test: $(TARGET_TEST)
	"./$(TARGET_TEST)"

clean:
	cat .gitignore | xargs rm -rf

doc: $(SRC_DIR)/* $(SRC_DIR)/**/*
	doxygen Doxyfile

lc:
	find "$(SRC_DIR)" -type f | sort -f | xargs wc -l

dist: $(TARGET).zip

$(TARGET).zip: $(TARGET)
	rm -rf "/tmp/$(TARGET)"
	mkdir -p "/tmp/$(TARGET)"
	cp -r "$(TARGET)" assets examples README.md "/tmp/$(TARGET)/"
	cd /tmp && \
	zip -rmT "$(shell pwd)/$(TARGET).zip" "$(TARGET)"

$(TARGET): $(OBJECTS_MAIN)
	cd "$(BUILD_DIR)" && \
	$(LD) -o "../$(TARGET)" $(OBJECTS_MAIN) $(LIBS)

$(TARGET_TEST): $(OBJECTS_TEST)
	cd "$(BUILD_DIR)" && \
	$(LD) -o "../$(TARGET_TEST)" $(OBJECTS_TEST) $(LIBS)

%.o: $(SRC_DIR)/%.cpp
	mkdir -p "$(BUILD_DIR)"
	$(CXX) $(CXXFLAGS) -c -o "$(BUILD_DIR)/$@" "$<"

%.o: $(SRC_DIR)/**/%.cpp
	mkdir -p "$(BUILD_DIR)"
	$(CXX) $(CXXFLAGS) -c -o "$(BUILD_DIR)/$@" "$<"

Makefile.d: $(SRC_DIR)/*.cpp $(SRC_DIR)/**/*.cpp
	$(CXX) -MM "$(SRC_DIR)"/*.cpp "$(SRC_DIR)"/**/*.cpp > Makefile.d

.PHONY: all clean compile compile_debug debug dist lc log run test

include Makefile.d
