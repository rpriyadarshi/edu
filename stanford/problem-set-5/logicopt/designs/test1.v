module alpha();
endmodule

module beta (o, i1, i2);
input i1, i2;
output o;

and u1 (.y(o), .a(i1), .b(i2));

endmodule

module gama (o1, o2, o3, i1, i2, i3) ;
input i1, i2, i3;
output o1, o2, o3;

//wire p1, p2, p3 ;

//assign p1 = 1'b0;
//assign i1 = 1'b1;

and u1 (.y(p2), .a(p1), .b(i3));
or u2 (.y(p1), .a(i1), .b(i2));
not u3 (.y(p3), .i(p2));
or u4 (.y(o1), .a(i1), .b(p3));
or u5 (.y(o2), .a(1'b0), .b(p3));
or u6 (.y(o3), .a(1'b1), .b(p3));

endmodule

