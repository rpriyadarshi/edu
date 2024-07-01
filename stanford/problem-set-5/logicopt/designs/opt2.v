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

