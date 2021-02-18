`timescale 1 ps / 1 ps

module tb_rtl_drawscreen();

    logic TB_CLOCK_50;
    logic [3:0] TB_KEY;
    logic [9:0] TB_SW;
    logic [9:0] TB_LEDR;
    logic [6:0] TB_HEX0;
    logic [6:0] TB_HEX1;
    logic [6:0] TB_HEX2;
    logic [6:0] TB_HEX3;
    logic [6:0] TB_HEX4;
    logic [6:0] TB_HEX5;
    logic [7:0] TB_VGA_R;
    logic [7:0] TB_VGA_G;
    logic [7:0] TB_VGA_B;
    logic TB_VGA_HS;
    logic TB_VGA_VS;
    logic TB_VGA_CLK;
    logic [7:0] TB_VGA_X;
    logic [6:0] TB_VGA_Y;
    logic [2:0] TB_VGA_COLOUR;
    logic TB_VGA_PLOT;
    integer i;
    integer curX;
    integer maxY;
    integer curI;

    drawscreen DUT(.CLOCK_50(TB_CLOCK_50), .KEY(TB_KEY), .SW(TB_SW), .LEDR(TB_LEDR), .HEX0(TB_HEX0),
              .HEX1(TB_HEX1), .HEX2(TB_HEX2), .HEX3(TB_HEX3), .HEX4(TB_HEX4), .HEX5(TB_HEX5), 
              .VGA_R(TB_VGA_R), .VGA_G(TB_VGA_G), .VGA_B(TB_VGA_B), .VGA_HS(TB_VGA_HS), .VGA_VS(TB_VGA_VS), 
              .VGA_CLK(TB_VGA_CLK), .VGA_X(TB_VGA_X), .VGA_Y(TB_VGA_Y), .VGA_COLOUR(TB_VGA_COLOUR), .VGA_PLOT(TB_VGA_PLOT));

    initial forever begin
        TB_CLOCK_50 = 0; #1;
        TB_CLOCK_50 = 1; #1;
    end
    initial begin
        TB_KEY[3] = 1'b0;
        TB_SW[2:0] = 3'b000;
        curX = 0;
        #5;
        TB_KEY[3] = 1'b1;
        TB_KEY[0] = 1'b0;
        for (i = 0; i <= 76800; i++) begin
            #1;#1;
        end

        $stop;
    end

endmodule: tb_rtl_drawscreen
