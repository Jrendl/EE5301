# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -Ofast
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: main.o input_parser.o sizer.o annealing_engine.o #all .o
	$(CC) $(CFLAGS) -o main main.o input_parser.o sizer.o annealing_engine.o
 
# The main.o target can be written more simply
 
main.o: main.cc input_parser.h sizer.h #dependency headers
	$(CC) $(CFLAGS) -c main.cc

input_parser.o: input_parser.h input_parser.cc
	$(CC) $(CFLAGS) -c input_parser.cc

sizer.o: sizer.h sizer.cc 
	$(CC) $(CFLAGS) -c sizer.cc

annealing_engine.o: annealing_engine.h annealing_engine.cc sizer.h
	$(CC) $(CFLAGS) -c annealing_engine.cc