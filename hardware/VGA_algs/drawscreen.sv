module drawscreen(input logic CLOCK_50, input logic [3:0] KEY,
             input logic [9:0] SW, output logic [9:0] LEDR,
             output logic [6:0] HEX0, output logic [6:0] HEX1, output logic [6:0] HEX2,
             output logic [6:0] HEX3, output logic [6:0] HEX4, output logic [6:0] HEX5,
             output logic [7:0] VGA_R, output logic [7:0] VGA_G, output logic [7:0] VGA_B,
             output logic VGA_HS, output logic VGA_VS, output logic VGA_CLK,
             output logic [8:0] VGA_X, output logic [7:0] VGA_Y,
             output logic [2:0] VGA_COLOUR, output logic VGA_PLOT);

    logic done_signal_blank;
    logic [8:0] vga_x_temp_blank;
    logic [7:0] vga_y_temp_blank;
    logic [2:0] set_colour_blank;
    logic vga_plot_temp_blank;

    logic done_signal_square;
    logic [8:0] vga_x_temp_square;
    logic [7:0] vga_y_temp_square;
    logic [2:0] set_colour_square;
    logic vga_plot_temp_square;
    logic start_the_square;

    logic [9:0] VGA_R_10;
    logic [9:0] VGA_G_10;
    logic [9:0] VGA_B_10;
    logic VGA_BLANK, VGA_SYNC;
    logic curr_player;

    logic [8:0] vga_x_temp;
    logic [7:0] vga_y_temp;
    logic [2:0] set_colour;
    logic vga_plot_temp;


    assign VGA_R = VGA_R_10[9:2];
    assign VGA_G = VGA_G_10[9:2];
    assign VGA_B = VGA_B_10[9:2];

    assign HEX0 = 7'b1111111;
    assign HEX1 = 7'b1111111;
    assign HEX2 = 7'b1111111;
    assign HEX3 = 7'b1111111;
    assign HEX4 = 7'b1111111;
    assign HEX5 = 7'b1111111;

    // assign VGA_X = vga_x_temp;
    // assign VGA_Y = vga_y_temp;
    // assign VGA_PLOT = vga_plot_temp;
    // assign VGA_COLOUR = set_colour;

    //##########################
    //TESTING ONLY
    //##########################

    enum {RESET, WAIT_FOR_START, PAINT_THE_STUFF_LEFT, PAINT_THE_STUFF_RIGHT, DONE} nextState, currState;

    assign vga_x_temp = done_signal_blank ? vga_x_temp_square : vga_x_temp_blank;
    assign vga_y_temp = done_signal_blank ? vga_y_temp_square : vga_y_temp_blank;
    assign set_colour = done_signal_blank ? set_colour_square : set_colour_blank;
    assign vga_plot_temp = done_signal_blank ? vga_plot_temp_square : vga_plot_temp_blank;


    assign LEDR[9:0] = 10'd0;

    blankboard screen(.clk(CLOCK_50), .rst_n(KEY[3]), .colour(SW[2:0]), .start(1'd1),
                      .done(done_signal_blank), .vga_x(vga_x_temp_blank), .vga_y(vga_y_temp_blank), .vga_colour(set_colour_blank),
                      .vga_plot(vga_plot_temp_blank));

    squaremapper square(.clk(CLOCK_50), .rst_n(KEY[3]), .colour(SW[2:0]), .start(start_the_square),
                      .x_square(SW[3:0]), .y_square(SW[7:4]), .player(curr_player), .attack_type(SW[9:8]), .done(done_signal_square), 
                      .vga_x(vga_x_temp_square), .vga_y(vga_y_temp_square), .vga_colour(set_colour_square),
                      .vga_plot(vga_plot_temp_square));

    vga_adapter#(.RESOLUTION("320x240")) vga_u0(.resetn(KEY[3]), .clock(CLOCK_50), .colour(set_colour),
                 .x(vga_x_temp), .y(vga_y_temp), .plot(vga_plot_temp),
                 .VGA_R(VGA_R_10), .VGA_G(VGA_G_10), .VGA_B(VGA_B_10),
                 .*);

    always_comb begin
        if (done_signal_blank == 1'd0) begin
            VGA_X = vga_x_temp_blank;
            VGA_Y = vga_y_temp_blank;
            VGA_PLOT = vga_plot_temp_blank;
            VGA_COLOUR = set_colour_blank;
        end else begin
            VGA_X = vga_x_temp_square;
            VGA_Y = vga_y_temp_square;
            VGA_PLOT = vga_plot_temp_square;
            VGA_COLOUR = set_colour_square;
        end
    end

    always @(posedge CLOCK_50 or negedge KEY[3]) begin
        if (KEY[3] == 0) begin
            currState <= RESET;           
        end
        else begin
            currState <= nextState;
        end
    end

    always_comb begin
        if (done_signal_blank) begin
            case (currState)
                RESET: begin
                    start_the_square = 0;
                    nextState = WAIT_FOR_START;
                    curr_player = 0;
                end
                WAIT_FOR_START: begin
                    start_the_square = 0;
                    if (KEY[0] == 0) begin
                        nextState = PAINT_THE_STUFF_LEFT;
                        curr_player = 0;
                    end else if (KEY[1] == 0) begin
                        nextState = PAINT_THE_STUFF_RIGHT;
                        curr_player = 1;
                    end
                    else begin
                        nextState = WAIT_FOR_START;
                        curr_player = 0;
                    end
                end
                PAINT_THE_STUFF_LEFT: begin
                    start_the_square = 1;
                    curr_player = 0;
                    if (done_signal_square == 1) begin
                        nextState = DONE;
                    end else begin
                        nextState = PAINT_THE_STUFF_LEFT;
                    end
                end
                PAINT_THE_STUFF_RIGHT: begin
                    start_the_square = 1;
                    curr_player = 1;
                    if (done_signal_square == 1) begin
                        nextState = DONE;
                    end else begin
                        nextState = PAINT_THE_STUFF_RIGHT;
                    end
                end
                DONE: begin
                    start_the_square = 1'd0;
                    curr_player = 0;
                    if (KEY[0] == 1 && KEY[1] == 1) begin
                        nextState = WAIT_FOR_START;
                    end else begin
                        nextState = DONE;
                    end
                end
                default: begin
                    start_the_square = 1'd0;
                    curr_player = 0;
                    nextState = RESET;
                end
            endcase

        end else begin
            start_the_square = 0;
            curr_player = 0;
            nextState = RESET;
        end
    end

endmodule: drawscreen
