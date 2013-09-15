# Sources
EXECUTABLE=main client.x

# Config
CC=gcc -Iinclude
CFLAGS= -Wall -Wextra -c -ggdb
LD=gcc
LDFLAGS= -lm

# Target

all: $(EXECUTABLE)

doxygen: Doxyfile
	@doxygen

run: main client.x cnf
	@./main cnf
	@cat out.txt

clean:
	@echo Cleaning
	@rm -f *.o *~
	@rm -f $(EXECUTABLE)
	@chmod 777 script/resremove
	@script/resremove
	@rm -rf doc out.txt

main: list.o util.o sem.o shm.o myio.o main.c
	@echo Building $@
	@$(CC) $(LDFLAGS) -o $@ $^

client.x: list.o util.o sem.o shm.o myio.o client.c
	@echo Building $@
	@ $(CC) $(LDFLAGS) -o $@ $^
	
util.o: lib/util.c include/util.h
	@echo $@
	@ $(CC) $(CFLAGS) -o $@ $<
	
sem.o: lib/sem.c include/sem.h 
	@echo $@
	@ $(CC) $(CFLAGS) -o $@ $<
	
shm.o: lib/shm.c include/shm.h 
	@echo $@
	@ $(CC) $(CFLAGS) -o $@ $<

list.o: lib/list.c include/list.h 
	@echo $@
	@ $(CC) $(CFLAGS) -o $@ $<
	
myio.o: lib/myio.c include/myio.h 
	@echo $@
	@ $(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean
