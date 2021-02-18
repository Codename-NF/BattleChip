module drawscreen(input logic CLOCK_50, input logic [3:0] KEY,
             input logic [9:0] SW, output logic [9:0] LEDR,
             output logic [6:0] HEX0, output logic [6:0] HEX1, output logic [6:0] HEX2,
             output logic [6:0] HEX3, output logic [6:0] HEX4, output logic [6:0] HEX5,
             output logic [7:0] VGA_R, output logic [7:0] VGA_G, output logic [7:0] VGA_B,
             output logic VGA_HS, output logic VGA_VS, output logic VGA_CLK,
             output logic [7:0] VGA_X, output logic [6:0] VGA_Y,
             output logic [2:0] VGA_COLOUR, output logic VGA_PLOT);

    logic done_signal;
    logic [8:0] vga_x_temp;
    logic [7:0] vga_y_temp;
    logic [2:0] set_colour;
    logic vga_plot_temp;

    logic [9:0] VGA_R_10;
    logic [9:0] VGA_G_10;
    logic [9:0] VGA_B_10;
    logic VGA_BLANK, VGA_SYNC;
    logic t2_start = 1'd1;

    assign VGA_R = VGA_R_10[9:2];
    assign VGA_G = VGA_G_10[9:2];
    assign VGA_B = VGA_B_10[9:2];

    assign HEX0 = 7'b1111111;
    assign HEX1 = 7'b1111111;
    assign HEX2 = 7'b1111111;
    assign HEX3 = 7'b1111111;
    assign HEX4 = 7'b1111111;
    assign HEX5 = 7'b1111111;

    assign VGA_X = vga_x_temp;
    assign VGA_Y = vga_y_temp;
    assign VGA_PLOT = vga_plot_temp;
    assign VGA_COLOUR = set_colour;

    assign LEDR[9:1] = 9'd0;

    blankboard screen(.clk(CLOCK_50), .rst_n(KEY[3]), .colour(SW[2:0]), .start(t2_start),
                      .done(LEDR[0]), .vga_x(vga_x_temp), .vga_y(vga_y_temp), .vga_colour(set_colour),
                      .vga_plot(vga_plot_temp));

    vga_adapter#(.RESOLUTION("320x240")) vga_u0(.resetn(KEY[3]), .clock(CLOCK_50), .colour(set_colour),
                 .x(vga_x_temp), .y(vga_y_temp), .plot(vga_plot_temp),
                 .VGA_R(VGA_R_10), .VGA_G(VGA_G_10), .VGA_B(VGA_B_10),
                 .*);

endmodule: drawscreen
