# Sources
EXECUTABLE=main

# Config
CC=gcc -Iinclude
CFLAGS= -Wall -c -ggdb
LD=gcc
LDFLAGS= -lpthread

# Target

all: $(EXECUTABLE)

doxygen: Doxyfile
	@doxygen

run: main cnf
	@./main cnf
	@cat out.txt

clean:
	@echo Cleaning
	@rm -f *.o *~
	@rm -f $(EXECUTABLE)
	@chmod 777 script/resremove
	@script/resremove
	@rm -rf doc out.txt

main: list.o util.o my_thread.o myio.o main.c
	@echo Building $@
	@$(CC) -o $@ $^ $(LDFLAGS) 

my_thread.o: lib/my_thread.c include/my_thread.h
	@echo $@
	@ $(CC) $(CFLAGS) -o $@ $< 
	
list.o: lib/list.c include/list.h 
	@echo $@
	@ $(CC) $(CFLAGS) -o $@ $<
	
util.o: lib/util.c include/util.h
	@echo $@
	@ $(CC) $(CFLAGS) -o $@ $<
	
myio.o: lib/myio.c include/myio.h 
	@echo $@
	@ $(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean
