module drawscreen(input logic clock, input logic reset_n, output logic [7:0] data_out,
             input logic [2:0] addr, input logic rd_en, input logic wr_en, input logic [7:0] data_in,
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

    logic [3:0] x_square;
    logic [3:0] y_square;
    logic [1:0] type_hit;
    logic player;

    assign VGA_R = VGA_R_10[9:2];
    assign VGA_G = VGA_G_10[9:2];
    assign VGA_B = VGA_B_10[9:2];

    // assign VGA_X = vga_x_temp;
    // assign VGA_Y = vga_y_temp;
    // assign VGA_PLOT = vga_plot_temp;
    // assign VGA_COLOUR = set_colour;

    //##########################
    //TESTING ONLY
    //##########################

    enum {RESET, GET_INSTR, WAIT_FOR_START, PAINT_THE_STUFF_LEFT, PAINT_THE_STUFF_RIGHT, DONE} nextState, currState;

    assign vga_x_temp = done_signal_blank ? vga_x_temp_square : vga_x_temp_blank;
    assign vga_y_temp = done_signal_blank ? vga_y_temp_square : vga_y_temp_blank;
    assign set_colour = done_signal_blank ? set_colour_square : set_colour_blank;
    assign vga_plot_temp = done_signal_blank ? vga_plot_temp_square : vga_plot_temp_blank;

    blankboard screen(.clk(clock), .rst_n(reset_n), .colour(3'd0), .start(1'd1),
                      .done(done_signal_blank), .vga_x(vga_x_temp_blank), .vga_y(vga_y_temp_blank), .vga_colour(set_colour_blank),
                      .vga_plot(vga_plot_temp_blank));

    squaremapper square(.clk(clock), .rst_n(reset_n), .colour(3'd0), .start(start_the_square),
                      .x_square(x_square), .y_square(y_square), .player(player), .attack_type(type_hit), .done(done_signal_square), 
                      .vga_x(vga_x_temp_square), .vga_y(vga_y_temp_square), .vga_colour(set_colour_square),
                      .vga_plot(vga_plot_temp_square));

    // vga_adapter#(.RESOLUTION("320x240")) vga_u0(.resetn(reset_n), .clock(clock), .colour(set_colour),
    //              .x(vga_x_temp), .y(vga_y_temp), .plot(vga_plot_temp),
    //              .VGA_R(VGA_R_10), .VGA_G(VGA_G_10), .VGA_B(VGA_B_10),
    //              .*);

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

    always @(posedge clock or negedge reset_n) begin
        if (reset_n == 0) begin
            currState <= RESET;           
        end
        else begin
            currState <= nextState;
        end
    end

    always @(posedge clock or negedge reset_n) begin
        if (reset_n == 0) begin
            x_square <= 0;
            y_square <= 0;
            type_hit <= 0;
            player <= 0;
            
        end
        else begin
            case (currState)
                RESET: begin
                    x_square <= 0;
                    y_square <= 0;
                    type_hit <= 0;
                    player <= 0;
                    start_the_square <= 0;
                    nextState <= GET_INSTR;
                end
                GET_INSTR: begin
                    data_out <= 0;
                    start_the_square <= 0;
                    if (addr == 4'd3 && wr_en == 1'd1) begin
                        player <= data_in[0];
                        nextState <= WAIT_FOR_START;   
                    end else if (addr == 4'd0 && wr_en == 1'd1) begin
                        x_square <= data_in[3:0];
                        nextState <= GET_INSTR; 
                    end else if (addr == 4'd1 && wr_en == 1'd1) begin
                        y_square <= data_in[3:0];
                        nextState <= GET_INSTR; 
                    end else if (addr == 4'd2 && wr_en == 1'd1) begin
                        type_hit <= data_in[1:0];
                        nextState <= GET_INSTR; 
                    end
                end
                WAIT_FOR_START: begin
                    data_out <= 0;
                    start_the_square <= 0;
                    if (done_signal_blank == 1) begin
                        nextState = PAINT_THE_STUFF_LEFT;
                    end else begin
                        nextState = WAIT_FOR_START;
                    end
                end
                PAINT_THE_STUFF_LEFT: begin
                    start_the_square <= 1;
                    if (done_signal_square == 1) begin
                        nextState <= DONE;
                    end else begin
                        nextState <= PAINT_THE_STUFF_LEFT;
                    end
                end
                DONE: begin
                    data_out <= 1;
                    start_the_square <= 1'd0;
                    curr_player <= 0;
                    if (addr == 4'd0 && wr_en == 1'd1) begin
                        x_square <= data_in[3:0];
                        nextState <= GET_INSTR; 
                    end else begin
                        nextState <= DONE;
                    end
                end
                default: begin
                    x_square <= 0;
                    y_square <= 0;
                    type_hit <= 0;
                    player <= 0;
                    start_the_square <= 0;
                    nextState <= GET_INSTR;
                end
            endcase
        end
    end

endmodule: drawscreen
