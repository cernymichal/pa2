CXX = g++
LD = g++
CXXFLAGS = -std=c++17 -Wall -pedantic -Wno-long-long -O2
LDFLAGS = #-lSDL2
OBJECTS = main.o

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
	$(LD) $(LDFLAGS) -o ../$(LOGIN) $(OBJECTS)

%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $(BUILD_DIR)/$@ $<

Makefile.d:
	$(CXX) -MM $(SRC_DIR)/*.cpp > Makefile.d

.PHONY: all clean compile run

include Makefile.d
