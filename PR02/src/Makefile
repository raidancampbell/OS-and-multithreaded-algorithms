CC = gcc
CFLAGS = -g -Wall
LIBS = # None yet...
OUTPUT = main car
HEADERS = main.h car.h

all: $(OUTPUT)

car: car.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

main: main.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

clean:
	@rm -f $(OUTPUT)
