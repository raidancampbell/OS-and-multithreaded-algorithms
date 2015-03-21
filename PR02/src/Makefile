CC = gcc
FLAGS = -g -Wall
LIBS = # none for now
SOURCE = car.c main.c
OUTPUT = main

all: $(SOURCE)
	@#Call the compiler with source & output args
	$(CC) $(LIBS) $(FLAGS) -o $(OUTPUT) $(SOURCE)
	@#Make the output file accessible
	chmod u+x $(OUTPUT)

clean:
	@# Using the '@' sign suppresses echoing
	@# the line while the command is run
	@rm -f $(OUTPUT)

CLEAN_MSG = "All resources closed/removed."
UNCLEAN_MSG = "Please clean resources using 'ipcrm'"

test: all
	@echo
	-@./$(OUTPUT)
	@echo
	@echo "Checking for open resources..."
	@ipcs | grep -q -w $(USER); if [ $$? = 1 ] ; then echo $(CLEAN_MSG); else ipcs && echo $(UNCLEAN_MSG); fi
