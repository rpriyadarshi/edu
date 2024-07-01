module hw3p2 (f, g, a, b, c);
input a, b, c;
output f, g;

and2 u1 (.y(Iu1), .a(a), .b(b));
or2 u2 (.y(Iu2), .a(a), .b(c));
or2 u3 (.y(Iu3), .a(b), .b(c));
or2 u4 (.y(f), .a(Iu1), .b(c));
and2 u5 (.y(g), .a(Iu2), .b(Iu3));

endmodule

