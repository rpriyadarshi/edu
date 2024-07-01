//////////////////////////////////////////////////////////////////////////////////
// Mult-add module
// 	This module computes one 4x8 element of a matrix multiplication
//////////////////////////////////////////////////////////////////////////////////
module multAdd4x8(
	input [0:7] a_pq, input [0:7] b_pq, 
	input [0:7] a_rs, input [0:7] b_rs, 
	output [0:16] c_mn
	);
	
	wire [0:15] ab_pq = a_pq*b_pq;
	wire [0:15] ab_rs = a_rs*b_rs;

	assign c_mn = ab_pq+ab_rs;
endmodule

//////////////////////////////////////////////////////////////////////////////////
// Top level module for problem 1.a
// 	4x4x8 matrix multiplication
//		Consider multiplication of two 2 × 2 matrices:
//			a1,1 a1,2   b1,1 b1,2   c1,1 c1,2
//			a2,1 a2,2 x b2,1 b2,2 = c2,1 c2,2 	- (1)
//		where ai,j’s and bi,j’s are 8-bit unsigned integers and ci,j’s are 
//		17 bit unsigned integers.
//////////////////////////////////////////////////////////////////////////////////
module matMult4x4x8(
	input [0:7] a_11, input [0:7] b_11, 
	input [0:7] a_12, input [0:7] b_12, 
	input [0:7] a_21, input [0:7] b_21, 
	input [0:7] a_22, input [0:7] b_22, 
	output [0:16] c_11,
	output [0:16] c_12,
	output [0:16] c_21,
	output [0:16] c_22
	);
	
	// Calculate all the elements of the matrix
	multAdd4x8 ma_11 (a_11, b_11, a_12, b_21, c_11);
	multAdd4x8 ma_12 (a_11, b_12, a_12, b_22, c_12);
	multAdd4x8 ma_21 (a_21, b_11, a_22, b_21, c_21);
	multAdd4x8 ma_22 (a_21, b_12, a_22, b_22, c_22);

endmodule

//////////////////////////////////////////////////////////////////////////////////
// Top level module for problem 1.a
// 	Design a combinational module at RTL using Verilog or VHDL that performs 
//		the above matrix multiplication. The module has eight 8-bit inputs and 
//		four 17-bit outputs. [10]
//////////////////////////////////////////////////////////////////////////////////
module p_1a(
	input [0:7] a_11, input [0:7] b_11, 
	input [0:7] a_12, input [0:7] b_12, 
	input [0:7] a_21, input [0:7] b_21, 
	input [0:7] a_22, input [0:7] b_22, 
	output [0:16] c_11,
	output [0:16] c_12,
	output [0:16] c_21,
	output [0:16] c_22
	);

	// Simply use the 4x4x8 matrix multiplication module.
	matMult4x4x8 mm4x4x8 (	
		a_11, b_11, a_12, b_12,
		a_21, b_21, a_22, b_22,	
		c_11, c_12, c_21, c_22
		);
		
endmodule

//////////////////////////////////////////////////////////////////////////////////
// Top level module for problem 1.b
// 	Modify the design in (a) so that the outputs are registered using positive 
//		edge triggered D flip-flops. Add an additional clock input to the 
//		design. [3]
//////////////////////////////////////////////////////////////////////////////////
module p_1b(
	input clk,
	input [0:7] a_11, input [0:7] b_11, 
	input [0:7] a_12, input [0:7] b_12, 
	input [0:7] a_21, input [0:7] b_21, 
	input [0:7] a_22, input [0:7] b_22, 
	output [0:16] c_11,
	output [0:16] c_12,
	output [0:16] c_21,
	output [0:16] c_22
	);

	wire [0:16] w_11, w_12, w_21, w_22;
	reg [0:16] r_11, r_12, r_21, r_22;
	
	// Push the result saved in the registers to output
	assign c_11 = r_11;
	assign c_12 = r_12;
	assign c_21 = r_21;
	assign c_22 = r_22;

	// Simply use the 4x4x8 matrix multiplication module, 
	// but the result is pushed to temporary wires.
	matMult4x4x8 mm4x4x8 (	
		a_11, b_11, a_12, b_12,
		a_21, b_21, a_22, b_22,	
		w_11, w_12, w_21, w_22
		);

	// At each positive edge of the clock, save the signal
	// transitioning the temporary wires to register.
	always @(posedge clk) begin
		r_11 <= w_11;
		r_12 <= w_12;
		r_21 <= w_21;
		r_22 <= w_22;
	end

endmodule

//////////////////////////////////////////////////////////////////////////////////
// Top level module for problem 1.c
//		Modify the design in (b) to have a synchronous load enable signal. When 
//		this signal is asserted, new values are loaded into the registers. 
//		Otherwise, the previous values are retained. [2]
//////////////////////////////////////////////////////////////////////////////////
module p_1c(
	input clk, input en, 
	input [0:7] a_11, input [0:7] b_11, 
	input [0:7] a_12, input [0:7] b_12, 
	input [0:7] a_21, input [0:7] b_21, 
	input [0:7] a_22, input [0:7] b_22, 
	output [0:16] c_11,
	output [0:16] c_12,
	output [0:16] c_21,
	output [0:16] c_22
	);

	wire [0:16] w_11, w_12, w_21, w_22;
	reg [0:16] r_11, r_12, r_21, r_22;
	
	// Push the result saved in the registers to output
	assign c_11 = r_11;
	assign c_12 = r_12;
	assign c_21 = r_21;
	assign c_22 = r_22;

	// Simply use the 4x4x8 matrix multiplication module, 
	// but the result is pushed to temporary wires.
	matMult4x4x8 mm4x4x8 (	
		a_11, b_11, a_12, b_12,
		a_21, b_21, a_22, b_22,	
		w_11, w_12, w_21, w_22
		);

	// At each positive edge of the clock, save the signal
	// transitioning the temporary wires to register, but
	// only if "en" is high
	always @(posedge clk) begin
		if (en) begin
			r_11 <= w_11;
			r_12 <= w_12;
			r_21 <= w_21;
			r_22 <= w_22;
		end
	end
endmodule

//////////////////////////////////////////////////////////////////////////////////
// Top level module for problem 1.d
//		Add a synchronous reset input to the design in (b). [3]
//////////////////////////////////////////////////////////////////////////////////
module p_1d(
	input clk, input rst, 
	input [0:7] a_11, input [0:7] b_11, 
	input [0:7] a_12, input [0:7] b_12, 
	input [0:7] a_21, input [0:7] b_21, 
	input [0:7] a_22, input [0:7] b_22, 
	output [0:16] c_11,
	output [0:16] c_12,
	output [0:16] c_21,
	output [0:16] c_22
	);

	wire [0:16] w_11, w_12, w_21, w_22;
	reg [0:16] r_11, r_12, r_21, r_22;
	
	// Push the result saved in the registers to output
	assign c_11 = r_11;
	assign c_12 = r_12;
	assign c_21 = r_21;
	assign c_22 = r_22;

	// Simply use the 4x4x8 matrix multiplication module, 
	// but the result is pushed to temporary wires.
	matMult4x4x8 mm4x4x8 (	
		a_11, b_11, a_12, b_12,
		a_21, b_21, a_22, b_22,	
		w_11, w_12, w_21, w_22
		);

	// At each positive edge of the clock, save the signal
	// transitioning the temporary wires to register, but
	// as long as "rst" is low. As soon as rst is set to
	// high and a positive clock edge happens, initialize 
	// the registers to "0". Since registers are 8 bits, 
	// use "8'b0"
	always @(posedge clk) begin
		if (rst) begin
			r_11 <= 8'b0;
			r_12 <= 8'b0;
			r_21 <= 8'b0;
			r_22 <= 8'b0;
		end
		else begin
			r_11 <= w_11;
			r_12 <= w_12;
			r_21 <= w_21;
			r_22 <= w_22;
		end
	end
endmodule

//////////////////////////////////////////////////////////////////////////////////
// Top level module for problem 1.e
//		Add an asynchronous reset input to design in (b). [2]
//////////////////////////////////////////////////////////////////////////////////
module p_1e(
	input clk, input rst, 
	input [0:7] a_11, input [0:7] b_11, 
	input [0:7] a_12, input [0:7] b_12, 
	input [0:7] a_21, input [0:7] b_21, 
	input [0:7] a_22, input [0:7] b_22, 
	output [0:16] c_11,
	output [0:16] c_12,
	output [0:16] c_21,
	output [0:16] c_22
	);

	wire [0:16] w_11, w_12, w_21, w_22;
	reg [0:16] r_11, r_12, r_21, r_22;

	// Push the result saved in the registers to output
	assign c_11 = r_11;
	assign c_12 = r_12;
	assign c_21 = r_21;
	assign c_22 = r_22;

	// Simply use the 4x4x8 matrix multiplication module, 
	// but the result is pushed to temporary wires.
	matMult4x4x8 mm4x4x8 (	
		a_11, b_11, a_12, b_12,
		a_21, b_21, a_22, b_22,	
		w_11, w_12, w_21, w_22
		);

	// At each positive edge of the clock, save the signal
	// transitioning the temporary wires to register, but
	// as long as "rst" is low. As soon as rst is set to
	// high, initialize the registers to "0" regardless of
	// the clock "clk". Since registers are 8 bits, use "8'b0"
	always @(posedge clk or posedge rst) begin
		if (rst) begin
			r_11 <= 8'b0;
			r_12 <= 8'b0;
			r_21 <= 8'b0;
			r_22 <= 8'b0;
		end
		else begin
			r_11 <= w_11;
			r_12 <= w_12;
			r_21 <= w_21;
			r_22 <= w_22;
		end
	end
endmodule
