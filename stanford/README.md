--------------------------------------------------------------------------------
PLATFORM: LINUX
--------------------------------------------------------------------------------
BUILDING-SYSTEM
--------------------------------------------------------------------------------
The build system consists of 4 scripts, and do what the name suggests.
	1. build-debug.sh
	2. build-release.sh
	3. clean-debug.sh
	4. clean-release.sh
--------------------------------------------------------------------------------
RUNNING
--------------------------------------------------------------------------------
	1. Run one of the build scripts ~ debug or release:
		$ ./build-release.sh
	2. Go the the directory "problem-set-5/logicopt/designs"
	   and link the binary locally:
		$ ln -s ../bin/Release/logicopt-bin
	3. Run the executable:
		$ ./logicopt-bin
		*****************************************
		* Welcome to the SIMPLE-LOGIC-OPTIMIZER *
		*****************************************
		LogicOpt> 

	4. Type:
		LogicOpt> help
	5. You should see the following help printed:
********************************************************************************
read_design <file-name>
	The read design command reads the specified text file containing the 
	gate level design and creates a run time model in memory

write_design <file-name>
	The write design command writes the current netlist in memory to a file. 

write_robdd <file-name>
	The write robdd command writes the current robdd in memory to a file. 

check_design <file-name>
	The check design command checks the netlist for such errors as dangling 
	nets, unconnected pins and the presence of combinational loops. 
	The command will print warnings indicating the problem and the 
	location of the problem. 

simplify
	(a) Constructs the BDD for each driver pin in the design. If the BDD for 
	    a driver pin shows that it is a constant function, then the pins 
	    driven by the driver pins are all disconnected from the driver pin 
	    and are connected to the appropriate constant pin. 
	(c') Constants must be propagated. 

clean_design
	(b) Any gates whose outputs are not connected must be deleted 
	    (recursively). 
	(c') Gates that are not needed should be eliminated. 

help
	This help message 
********************************************************************************
	6. The command simplify does not clean up the design. You need to run
	   clean_design after simplify. The reason for this deviation is to 
	   allow dumping the ROBDD contents. by the following command:
		LogicOpt> write_robdd <file-name>
	   It writes a syntax similar to verilog with embedded ROBDD for each
	   gate.
	7. You can also use 2 extra commands 'ls' and 'pwd' to see what you have
 	   and where you are to assist typing the filenames correctly.

	8. You can use 'exit' to exit the tool.

--------------------------------------------------------------------------------
TEST-CASES
--------------------------------------------------------------------------------
The testcases shown below can be run in the system:
	1. and2.v	A simple and gate.
	2. or2.v	A simple or gate.
	3. xor2.v	A simple xor gate.
	4. hw3p2.v	Home-work 3 problem 2
	5. test1.v	A bit more complicated test, with multiple modules.
	6. test2.v	More complex testcase, for constant propagation.
	7. test3.v	Exercises the validity of the design.
	8. verilog1.v	Your example 1.
	9. verilog2.v	Your example 2.

--------------------------------------------------------------------------------
TEST-RESULTS
--------------------------------------------------------------------------------
Testcase 8. verilog1.v
--------------------------------------------------------------------------------
module myckt1 (a, b, o);
input a, b;
output o;

wire w1, w2, wx;

or2 R_1 (.y(w1), .a(a), .b(w2));
or2 R_2 (.y(w2), .a(b), .b(w1));
and2 A_1 (.y(wx), .a(w2));

endmodule
--------------------------------------------------------------------------------
*****************************************
* Welcome to the SIMPLE-LOGIC-OPTIMIZER *
*****************************************
LogicOpt> read_design verilog1.v
RUNNING: read_design <verilog1.v>
LogicOpt> check_design log1.dat
RUNNING: check_design <log1.dat>
LogicOpt> exit
Goodbye!

log1.dat:
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WARNING: Unconnected pin "b", in instance and2 A_1(.y(wx), .a(w2));
WARNING: Dangling net "o" has no driver
WARNING: Dangling net "wx" drives nothing
WARNING: Found loop at gate R_2(.b(w1))
WARNING: Found loop at gate R_1(.b(w2))
WARNING: Found loop at gate A_1(.a(w2))
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
--------------------------------------------------------------------------------
Testcase 9. verilog1.v
--------------------------------------------------------------------------------
module myckt2 (o1, o2, o3, a, b, c, d);
input a, b, c, d;
output o1, o2, o3;

wire na, nc, nanc, a_c, b_c, a_cb_c, ab, ab_c, z, zb;

not N_1 (.y(na), .i(a));
not N_2 (.y(nc), .i(c));
and2 A_1 (.y(nanc), .a(na), .b(nc));
or2 R_1 (.y(a_c), .a(a), .b(c));
and2 A_2 (.y(o1), .a(nanc), .b(a_c));
or2 R_2 (.y(b_c), .a(b), .b(c));
not N_3 (.y(o2), .i(b_c));
and2 A_3 (.y(a_cb_c), .a(b_c), .b(a_c));
and2 A_4 (.y(ab), .a(a), .b(b));
or2 R_3 (.y(ab_c), .a(ab), .b(c));
xor2 X_1 (.y(z), .a(a_cb_c), .b(ab_c));
not N_4 (.y(zb), .i(z));
xor2 X_2 (.y(o3), .a(zb), .b(d));

endmodule
--------------------------------------------------------------------------------
*****************************************
* Welcome to the SIMPLE-LOGIC-OPTIMIZER *
*****************************************
LogicOpt> read_design verilog2.v
RUNNING: read_design <verilog2.v>
LogicOpt> check_design log2.v
RUNNING: check_design <log2.v>
LogicOpt> simplify
RUNNING: simplify
Found ZERO: and2 A_2 pin (.y(o1)) net (o1)
Found ZERO: xor2 X_1 pin (.y(z)) net (z)
Found ONE: not N_4 pin (.y(zb)) net (zb)
LogicOpt> clean_design
RUNNING: clean_design
REMOVING INSTANCE: and2 A_2();
REMOVING INSTANCE: not N_4();
REMOVING INSTANCE: xor2 X_1();
REMOVING INSTANCE: xor2 X_2();
REMOVING NET: a_cb_c
REMOVING NET: ab_c
REMOVING NET: d
REMOVING NET: nanc
REMOVING NET: o1
REMOVING NET: z
REMOVING NET: zb
REMOVING INSTANCE: and2 A_1();
REMOVING INSTANCE: and2 A_3();
REMOVING INSTANCE: or2 R_3();
REMOVING NET: a_c
REMOVING NET: ab
REMOVING NET: na
REMOVING NET: nc
REMOVING INSTANCE: and2 A_4();
REMOVING INSTANCE: not N_1();
REMOVING INSTANCE: not N_2();
REMOVING INSTANCE: or2 R_1();
REMOVING NET: a
LogicOpt> write_design opt2.v
RUNNING: write_design <opt2.v>
LogicOpt> exit
Goodbye!

log2.dat:
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

opt2.v
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
module myckt2 (o1, o2, o3, a, b, c, d);
output o1;
output o2;
output o3;
input a;
input b;
input c;
input d;

wire b_c;

not N_3(.y(o2), .i(b_c));
or2 R_2(.y(b_c), .a(b), .b(c));
not not_X_2(.y(o3), .i(1'b1));
one one(.y(1'b1));
zero zero(.y(1'b0));

endmodule


