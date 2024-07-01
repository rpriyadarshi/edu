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
