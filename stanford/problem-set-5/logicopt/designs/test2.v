module alpha (o, a, b, c, d, e, f);
input a, b, c, d, e, f;
output o;

//and u1 (.y(i1), .a(1'b0), .b(b));
and u1 (.y(i1), .a(a), .b(b));

and u2 (.y(i2), .a(i1), .b(c));

and u3 (.y(i3), .a(i2), .b(1'b0));
//and u3 (.y(i3), .a(i2), .b(d));

and u4 (.y(i4), .a(i3), .b(e));

and u5 (.y(o), .a(i4), .b(f));

endmodule

