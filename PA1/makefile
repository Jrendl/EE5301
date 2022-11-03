# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
sta: sta.o gate_lib.o circuit_parser.o timing_analyzer.o
	$(CC) $(CFLAGS) -o sta sta.o gate_lib.o circuit_parser.o timing_analyzer.o
 
# The main.o target can be written more simply
 
sta.o: sta.cc gate_lib.h circuit_parser.h timing_analyzer.h
	$(CC) $(CFLAGS) -c sta.cc
 
gate_lib.o: gate_lib.h
 
circuit_parser.o: circuit_parser.h gate_lib.h

timing_analyzer.o : timing_analyzer.h circuit_parser.h gate_lib.h