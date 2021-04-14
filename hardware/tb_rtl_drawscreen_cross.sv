`timescale 1 ps / 1 ps
`define BASEX_LEFT 9'd10
`define BASEX_RIGHT 9'd178
`define BASEY 8'd91

module tb_rtl_drawscreen_cross();

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
    logic [8:0] TB_VGA_X;
    logic [7:0] TB_VGA_Y;
    logic [2:0] TB_VGA_COLOUR;
    logic TB_VGA_PLOT;
    integer i;
    integer curX;
    integer curY;
    integer curI;

    drawscreen_cross DUT(.CLOCK_50(TB_CLOCK_50), .KEY(TB_KEY), .SW(TB_SW), .LEDR(TB_LEDR), .HEX0(TB_HEX0),
              .HEX1(TB_HEX1), .HEX2(TB_HEX2), .HEX3(TB_HEX3), .HEX4(TB_HEX4), .HEX5(TB_HEX5), 
              .VGA_R(TB_VGA_R), .VGA_G(TB_VGA_G), .VGA_B(TB_VGA_B), .VGA_HS(TB_VGA_HS), .VGA_VS(TB_VGA_VS), 
              .VGA_CLK(TB_VGA_CLK), .VGA_X(TB_VGA_X), .VGA_Y(TB_VGA_Y), .VGA_COLOUR(TB_VGA_COLOUR), .VGA_PLOT(TB_VGA_PLOT));

    initial forever begin
        TB_CLOCK_50 = 0; #1;
        TB_CLOCK_50 = 1; #1;
    end
    initial begin
        TB_KEY[3] = 1'b0;
        TB_SW[3:0] = 4'd5;
        TB_SW[7:4] = 4'd5;
        TB_SW[9:8] = 2'd0;
        curX = 0;
        #5;
        TB_KEY[3] = 1'b1;
        TB_KEY[0] = 1'b1;
        TB_KEY[1] = 1'b1;

        //Print blankboard
        for (i = 0; i <= 76800; i++) begin
            #1;#1;
        end
        //$stop;

        //Print (5,5) to "MISS" on board 1
        TB_KEY[0] = 1'b0;
        #6;
        print_square(TB_SW[3:0], TB_SW[7:4], 3'b111, 0);
        TB_KEY[0] = 1'b1;
        #10;
        $stop;
        #2;

        //Print (5,5) to "MISS" on board 2
        TB_KEY[1] = 1'b0;
        #6;
        print_square(TB_SW[3:0], TB_SW[7:4], 3'b111, 1);
        TB_KEY[1] = 1'b1;
        #2;
        TB_SW[9:8] = 2'd1;
        TB_SW[3:0] = 4'd0;
        TB_SW[7:4] = 4'd9;
        #8;
        //$stop;
        #2;

        //Print (0,9) to "HIT" on board 1
        TB_KEY[0] = 1'b0;
        #6;
        print_square(TB_SW[3:0], TB_SW[7:4], 3'b100, 0);
        TB_KEY[0] = 1'b1;
        #2;
        TB_SW[3:0] = 4'd0;
        TB_SW[7:4] = 4'd0;
        #8;
        //$stop;
        #2;

        //Print (0,0) to "HIT" on board 1
        TB_KEY[0] = 1'b0;
        #6;
        print_square(TB_SW[3:0], TB_SW[7:4], 3'b100, 0);
        TB_KEY[0] = 1'b1;
        #2;
        TB_SW[3:0] = 4'd9;
        TB_SW[7:4] = 4'd0;
        #8;
        //$stop;
        #2;

        //Print (9,0) to "HIT" on board 1
        TB_KEY[0] = 1'b0;
        #6;
        print_square(TB_SW[3:0], TB_SW[7:4], 3'b100, 0);
        TB_KEY[0] = 1'b1;
        #2;
        TB_SW[3:0] = 4'd9;
        TB_SW[7:4] = 4'd9;
        #8;
        //$stop;
        #2;

        //Print (9,9) to "HIT" on board 1
        TB_KEY[0] = 1'b0;
        #6;
        print_square(TB_SW[3:0], TB_SW[7:4], 3'b100, 0);
        TB_KEY[0] = 1'b1;
        #2;

        //Same process for board 2, try magenta

        TB_SW[9:8] = 2'd2;
        TB_SW[3:0] = 4'd0;
        TB_SW[7:4] = 4'd9;
        #8;
        //$stop;
        #2;

        //Print (0,9) to "HIT" on board 2
        TB_KEY[1] = 1'b0;
        #6;
        print_square(TB_SW[3:0], TB_SW[7:4], 3'b101, 1);
        TB_KEY[1] = 1'b1;
        #2;
        TB_SW[3:0] = 4'd0;
        TB_SW[7:4] = 4'd0;
        #8;
        //$stop;
        #2;

        //Print (0,0) to "HIT" on board 2
        TB_KEY[1] = 1'b0;
        #6;
        print_square(TB_SW[3:0], TB_SW[7:4], 3'b101, 1);
        TB_KEY[1] = 1'b1;
        #2;
        TB_SW[3:0] = 4'd9;
        TB_SW[7:4] = 4'd0;
        #8;
        //$stop;
        #2;

        //Print (9,0) to "HIT" on board 2
        TB_KEY[1] = 1'b0;
        #6;
        print_square(TB_SW[3:0], TB_SW[7:4], 3'b101, 1);
        TB_KEY[1] = 1'b1;
        #2;
        TB_SW[3:0] = 4'd9;
        TB_SW[7:4] = 4'd9;
        #8;
        //$stop;
        #2;

        //Print (9,9) to "HIT" on board 2
        TB_KEY[1] = 1'b0;
        #6;
        print_square(TB_SW[3:0], TB_SW[7:4], 3'b101, 1);
        TB_KEY[1] = 1'b1;
        #4;







        $stop;
    end

task print_square;
    input [3:0] x_val;
    input [3:0] y_val;
    input [2:0] expected_colour;
    input player;
    begin
        #2;
        for (i = 0; i <= 11; i++) begin
            if (player == 0) begin
                curX = `BASEX_LEFT + (14 * x_val) + (i % 12);
            end else begin
                curX = `BASEX_RIGHT + (14 * x_val) + (i % 12);
            end
            curY = `BASEY + (14 * TB_SW[7:4]) + (i % 12);

            $display("Cycle: %d", i);
            assert(TB_VGA_X == (curX)) else $display("The X is currently %d when should be %d", TB_VGA_X,  curX);
            assert(TB_VGA_Y == (curY)) else $display("The Y is currently %d when should be %d", TB_VGA_Y,  curY);
            #1;#1;

            //assert(TB_VGA_COLOUR == expected_colour);
            //assert(TB_VGA_PLOT == 1);
        end
        #2;

        for (i = 0; i <= 11; i++) begin

            if (player == 0) begin
                curX = `BASEX_LEFT + (14 * x_val) - (i % 12) + 11;
            end else begin
                curX = `BASEX_RIGHT + (14 * x_val) - (i % 12) + 11;
            end
            curY = `BASEY + (14 * TB_SW[7:4]) + (i % 12);
            $display("Cycle: %d", i+12);
            assert(TB_VGA_X == (curX)) else $display("The X is currently %d when should be %d", TB_VGA_X,  curX);
            assert(TB_VGA_Y == (curY)) else $display("The Y is currently %d when should be %d", TB_VGA_Y,  curY);
            #1;#1;

            //assert(TB_VGA_COLOUR == expected_colour);
            //assert(TB_VGA_PLOT == 1);
        end
    end
endtask

endmodule: tb_rtl_drawscreen_cross
