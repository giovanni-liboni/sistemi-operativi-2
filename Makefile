
#     This file is part of sistemi-operativi-2
#     (C) 2013 Giovanni Liboni

#     This library is free software; you can redistribute it and/or
#     modify it under the terms of the GNU Lesser General Public
#     License as published by the Free Software Foundation; either
#     version 2.1 of the License, or (at your option) any later version.

#     This library is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#     Lesser General Public License for more details.
#
#     You should have received a copy of the GNU Lesser General Public
#     License along with this library; if not, write to the Free Software
#     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#
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
