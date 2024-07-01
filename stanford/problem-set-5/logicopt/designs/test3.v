module alpha (o, a, b, c, d, e, f);
input a, b, c, d, e, f;
output o;

wire x, y, z;
wire m, n;

and u1 (.y(i1), .a(), .b(b));
and u2 (.y(i2), .a(i1), .b(c));
and u3 (.y(i3), .a(), .b(d));
and u4 (.y(i4), .a(i3), .b(e));
and u5 (.y(o), .a(i4));

and u2 (.y(m), .a(a), .b(n));
and u3 (.y(n), .a(m), .b(c));

endmodule

