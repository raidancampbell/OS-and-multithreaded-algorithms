CC = clang
CFLAGS = -g -Wall
LIBS = # None yet...
OUTPUT = main smoker.bin agent.bin
HEADERS = main.h

all: $(OUTPUT)

car.bin: car.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

main: main.c $(HEADERS)
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

clean:
	@rm -f $(OUTPUT)