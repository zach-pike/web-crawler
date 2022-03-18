OBJECTS = $(patsubst %.cpp,%.o, $(shell find src/ -name *.cpp))
CC = g++
CFLAGS = -c -std=c++17 -I./src/ -g
LDFLAGS = -pthread -lssl -lcrypto -g

all: $(OBJECTS)

build: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o build.elf

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS)