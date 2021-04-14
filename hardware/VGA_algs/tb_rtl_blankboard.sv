module tb_rtl_blankboard();

    logic TB_CLOCK_50;
    logic TB_rst_n;
    logic [2:0] TB_colour;
    logic TB_start;
    logic TB_done;
    logic [7:0] TB_x;
    logic [6:0] TB_y;
    logic [2:0] TB_colour_out;
    logic TB_plot;
    integer i;
    integer curX;
    integer maxY;
    integer curI;
    //integer maxX;

    blankboard screen(.clk(TB_CLOCK_50), .rst_n(TB_rst_n), .colour(TB_colour), .start(TB_start),
                          .done(TB_done), .vga_x(TB_x), .vga_y(TB_y), .vga_colour(TB_colour_out),
                          .vga_plot(TB_plot));

    initial forever begin
        TB_CLOCK_50 = 0; #1;
        TB_CLOCK_50 = 1; #1;
    end
    initial begin
        TB_rst_n = 1'b0;
        TB_colour = 3'b000;
        #5;
        curX = 0;
        maxY = 0;
        curI = 0;
        TB_rst_n = 1'b1;
        TB_start = 1'b1;
        for (i = 0; i <= 76800; i++) begin
            #1;#1;
        end
        assert(TB_done === 1'd1);
        assert(TB_plot === 0);
        #3;
        TB_start = 1'd0;
        #3;
        assert(TB_done == 1'd0);
        assert(TB_plot === 0);

        $stop;
    end

endmodule: tb_rtl_blankboard
