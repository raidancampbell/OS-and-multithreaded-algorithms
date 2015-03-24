CC = gcc
CFLAGS = -g -Wall
LIBS = -pthread
SOURCE = main.c
OUTPUT = main

all: $(SOURCE)
	$(CC) $(LIBS) $(CFLAGS) -o $(OUTPUT) $(SOURCE)

clean:
	@rm -f $(OUTPUT)
