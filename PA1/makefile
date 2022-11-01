# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
partb: partb.o gate_lib.o circuit_parser.o
	$(CC) $(CFLAGS) -o partb partb.o gate_lib.o circuit_parser.o
 
# The main.o target can be written more simply
 
partb.o: partb.cc gate_lib.h circuit_parser.h
	$(CC) $(CFLAGS) -c partb.cc
 
gate_lib.o: gate_lib.h
 
circuit_parser.o: circuit_parser.h gate_lib.h