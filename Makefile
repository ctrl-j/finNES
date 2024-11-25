CC = g++
CFLAGS = -std=c++20 -Wall -Werror -g -pedantic

SOURCES = $(wildcard *.cpp) $(wildcard */*/*.cpp)
#SOURCES = src/main.cpp src/display.cpp src/glad.c

INCLUDE_PATHS = -I. -I/include

# Windows library/include locations
ifeq ($(OS), Windows_NT)
	LDFLAGS = -LC:/msys64/ucrt64/lib -LC:/msys64/ucrt64/lib/ctrlj
	LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -static -lpthread 
	INCLUDE_PATHS += -IC:/msys64/ucrt64/include -IC:/msys64/ucrt64/include/ctrlj
# Linux/WSL library/include locations
else
	LDFLAGS = -L/usr/local/lib
	LDLIBS = -lglfw3 -lGL -lm -lpthread -ldl -lrt -lX11 -lXrandr -lXi
	INCLUDE_PATHS += -I/usr/local/include

endif

all: finNES

finNES: $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS)

.PHONY: test

test: $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -DTEST