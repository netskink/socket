
# One difference of this makefile over the others is that it has two targets.
# This means that the makefile must adjust so it only builds the code for 
# each target with the corresponding main routine.

TARGET_SRV = server
TARGET_CLI = client
CC = gcc

# Debug Flags
CFLAGS = -g -Wall
LFLAGS = -g -Wall
LIBS = -lm

# Profile Flags
#CFLAGS = -g -Wall -pg
#LFLAGS = -pg 


.PHONY: default all clean test

default: $(TARGET_SRV) $(TARGET_CLI)
all: default

# OBJECTS uses the filter-out mechanism above to remove the files which have main routines 
# in them.  The example above was doing it at the object level.
MAINS = main_$(TARGET_SRV).c main_$(TARGET_CLI).c
OBJECTS = $(patsubst %.c, %.o, $(filter-out  $(MAINS) , $(wildcard *.c))    ) 
HEADERS = $(wildcard *.h)



%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET_SRV) $(TARGET_CLI) $(OBJECTS)

$(TARGET_SRV): $(OBJECTS) main_$(TARGET_SRV).o
	$(CC) -o $@ $^ $(LFLAGS) $(LIBS) 

$(TARGET_CLI): $(OBJECTS) main_$(TARGET_CLI).o
	$(CC) -o $@ $^ $(LFLAGS) $(LIBS) 



clean:
	-rm -f *.o
	-rm -f $(TARGET_SRV)
	-rm -f $(TARGET_CLI)

test: all
	./$(TARGET_SRV) 












