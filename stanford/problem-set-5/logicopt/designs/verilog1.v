module myckt1 (a, b, o);
input a, b;
output o;

wire w1, w2, wx;

or2 R_1 (.y(w1), .a(a), .b(w2));
or2 R_2 (.y(w2), .a(b), .b(w1));
and2 A_1 (.y(wx), .a(w2));

endmodule

