# 	Exercicio 3 de Sistemas Operativos

#	Grupo 50:
#		Sara Machado Nr 86923
#		Ricardo Grade Nr 90774

SOURCES_1 = CircuitRouter-AdvShell.c
SOURCES_1+= ../lib/commandlinereader.c ../lib/stringmanipulator.c
OBJS_1 = $(SOURCES_1:%.c=%.o)
TARGET_1 = CircuitRouter-AdvShell

SOURCES_2 = CircuitRouter-Client.c
SOURCES_2+= ../lib/stringmanipulator.c
OBJS_2 = $(SOURCES_2:%.c=%.o)
TARGET_2 = CircuitRouter-Client

SOURCES = CircuitRouter-AdvShell.c CircuitRouter-Client.c
SOURCES+= ../lib/commandlinereader.c ../lib/stringmanipulator.c
OBJS = $(SOURCES:%.c=%.o)

CC   = gcc
CFLAGS =-Wall -std=gnu99 -I../
LDFLAGS=-lm

all: $(TARGET_1) $(TARGET_2)

$(TARGET_1): $(OBJS_1)
	$(CC) $(CFLAGS) $^ -o $(TARGET_1) $(LDFLAGS)

$(TARGET_2): $(OBJS_2)
	$(CC) $(CFLAGS) $^ -o $(TARGET_2) $(LDFLAGS)

CircuitRouter-AdvShell.o: CircuitRouter-AdvShell.c CircuitRouter-AdvShell.h ../lib/commandlinereader.h ../lib/stringmanipulator.h ../lib/timer.h ../lib/types.h
CircuitRouter-Client.o: CircuitRouter-Client.c CircuitRouter-Client.h ../lib/stringmanipulator.h
../lib/commandlinereader.o: ../lib/commandlinereader.c ../lib/commandlinereader.h
../lib/stringmanipulator.o: ../lib/stringmanipulator.c ../lib/stringmanipulator.h

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo Cleaning...
	rm -f $(TARGET_1) $(TARGET_2) $(OBJS)
