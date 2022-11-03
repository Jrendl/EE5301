# EE 5301 Programing Assignment 1 #
Joe Rendleman - Prof: Kia Bazargan

## Assumptions ##
* Since the liberty file only contains 2-input gates, for an n-input gate (with the same logic function) in a circuit under test, multiply the entry from the LUT with (n/2) to obtain the corresponding delay/slew values1.  
* The input capacitance of the n-input gate may be assumed to remain unchanged from that of the corresponding 2-input gate.  
* No need to differentiate between rise and fall transitions and you can assume the same delay/slew LUTs for both. You can also assume the same delay/slew LUTs for all the input-output paths 
within the gate
* The arrival times at each primary input is 0, and input slew is 2 picoseconds (ps).  
* The load capacitance of each gate is equal to the sum of capacitance of each of its fanout gates (i.e., ignore any wire capacitance). 
* The load capacitance of the final stage of gates (which are simply connected to the primary outputs) is equal to four times the capacitance of an inverter from the liberty file.  
* The required arrival time is 1.1 times the total circuit delay, and is the same at each primary output of the circuit.
* The values given for the lookup of the delay and slew are always between the max index and min index values


## File Descriptions ##

* ``circuit_parser.cc`` - functions from PA0 that parse the circuit file and create fanin and fanout lists
* ``gate_lib.cc`` - functions from PA0 that parse the liberty file and save gates in a datastructure
* ``timing_analyzer.cc`` - functions to perform sta on a circuit which was parsed by ``circuit_parser``. 
* ``sta.cc`` - main program
* ``makeFile`` - makes project with dependencies
* `` circuit_parser.h``, ``gate_lib.h``, ``timing_analyzer.h`` - header files defining the classes in which the functions of their corresponding source files are encapsulated.

## known issues ##
* ``circuit_parser::parse_circuit_file()`` is duplicating some gates when creating the adjacency lists. 
    * Steps taken:
        * stepped through with gdb
        * changed delimiters and attempted to parse them out in code
        * changed way of iterating through words in file
    
* ``timing_analyzer::backward_traverse()`` seems to be calculating slack wrong. Getting negative values.
    * steps taken:
        * originally had arrival time and required time swapped, I corrected this.
        * used gdb to attempt to find the error.
* ``timing_analyzer::output()`` is causing a seg-fault once it attempts to retrace the critical path. I believe this is resultant from the previous two errors. 

## Note ##

My failures on this project are not for a lack of trying. I've spent at least 20 hours writing and debugging these issues and I still cannot find a good solution. For sake of my mental health and my other classes I am leaving this project here. The algorithms should (as far as I can tell) be theoretically complete but I'm obviously missing something.