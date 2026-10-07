CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRC      = $(wildcard src/*.cpp)
OUT      = cat_shell

ifeq ($(OS),Windows_NT)
    OUT := $(OUT).exe
endif

.PHONY: all clean

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	$(RM) $(OUT) cat_shell.exe
