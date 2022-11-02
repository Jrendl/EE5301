# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: main.o gate_lib.o circuit_parser.o
	$(CC) $(CFLAGS) -o main main.o gate_lib.o circuit_parser.o
 
# The main.o target can be written more simply
 
main.o: main.cc gate_lib.h circuit_parser.h
	$(CC) $(CFLAGS) -c main.cc
 
gate_lib.o: gate_lib.h
 
circuit_parser.o: circuit_parser.h gate_lib.h