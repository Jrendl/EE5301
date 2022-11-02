# EE 5301 Programing Assignment 1 #
Joe Rendleman

## Assumptions ##
* Since the liberty file only contains 2-input gates, for an n-input gate (with the same logic function) in a circuit under test, multiply the entry from the LUT with (n/2) to obtain the corresponding delay/slew values1.  
* The input capacitance of the n-input gate may be assumed to remain unchanged from that of the corresponding 2-input gate.  
* No need to differentiate between rise and fall transitions and you can assume the same delay/slew LUTs for both. You can also assume the same delay/slew LUTs for all the input-output paths 
within the gate
* The arrival times at each primary input is 0, and input slew is 2 picoseconds (ps).  
* The load capacitance of each gate is equal to the sum of capacitance of each of its fanout gates (i.e., ignore any wire capacitance). 
* The load capacitance of the final stage of gates (which are simply connected to the primary outputs) is equal to four times the capacitance of an inverter from the liberty file.  
* The required arrival time is 1.1 times the total circuit delay, and is the same at each primary output of the circuit.


