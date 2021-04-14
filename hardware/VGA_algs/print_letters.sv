`define RESET 3'd0
`define LEFTP 3'd1
`define LEFTONE 3'd2
`define RIGHTP 3'd3
`define RIGHTONE 3'd4
`define DONE 3'd4

module print_letters(input logic clk, input logic rst_n, input logic [2:0] colour,
                  input logic start, output logic done,
                  output logic [8:0] vga_x, output logic [7:0] vga_y,
                  output logic [2:0] vga_colour, output logic vga_plot);

    logic line_start;
    logic line_plot;
    logic line_done;

    logic [8:0] curr_x;
    logic [7:0] start_y;
    logic [7:0] end_y;

    logic [2:0] currState;
    logic [2:0] nextState;

    assign vga_colour = colour;


    drawline curr_line(clk, rst_n, 3'b010, curr_x, start_y, curr_x, end_y, line_start, line_done, vga_x, vga_y, vga_colour, line_plot);    

    always @(posedge clk or negedge rst_n) begin
        if (rst_n == 0) begin
            currState <= `RESET;           
        end
        else if (start) begin
            currState <= nextState;
        end else if (start == 0) begin
            currState <= `RESET;
        end
    end

    always @(posedge clk or negedge rst_n) begin
        if (rst_n == 0) begin
            curr_x = 9'd20;
            start_y = 8'd20;
            end_y = 8'd73;        
        end
        else if (currState == `RESET) begin
            curr_x = 9'd20;
            start_y = 8'd20;
            end_y = 8'd73;  
        end else if (start == 0) begin
            currState <= `RESETR;
        end
    end

    always_comb begin
        if (start) begin
            case(currState)
                `RESET: begin
                    t_start = 1'd0;
                    l_start = 1'd0;
                    r_start = 1'd0;
                    vga_plot = 1'd0;
                    vga_x = 8'd0;
                    vga_y = 8'd0;
                    nextState = `TOPR;
                end
                `LEFTP: begin
                    t_start = 1'd1;
                    l_start = 1'd0;
                    r_start = 1'd0;
                    vga_x = curr_x_t;
                    vga_y = curr_y_t;
                    vga_plot = plot1;
                    if (t_done) begin
                        nextState = `RIGHTR;
                    end else begin
                        nextState = `TOPR;
                    end
                end
                `LEFTONE: begin
                    t_start = 1'd1;
                    l_start = 1'd0;
                    r_start = 1'd1;
                    vga_x = curr_x_r;
                    vga_y = curr_y_r;
                    vga_plot = plot2;
                    if (r_done) begin
                        nextState = `LEFTR;
                    end else begin
                        nextState = `RIGHTR;
                    end
                end
                `RIGHTP: begin
                    t_start = 1'd1;
                    l_start = 1'd1;
                    r_start = 1'd1;
                    vga_x = curr_x_l;
                    vga_y = curr_y_l;
                    vga_plot = plot3;
                    if (l_done) begin
                        nextState = `DONER;
                    end else begin
                        nextState = `LEFTR;
                    end
                end
                `RIGHTONE: begin
                    t_start = 1'd1;
                    l_start = 1'd1;
                    r_start = 1'd1;
                    vga_x = curr_x_l;
                    vga_y = curr_y_l;
                    vga_plot = plot3;
                    if (l_done) begin
                        nextState = `DONER;
                    end else begin
                        nextState = `LEFTR;
                    end
                end
                `DONE: begin
                    t_start = 1'd1;
                    l_start = 1'd1;
                    r_start = 1'd1;
                    vga_x = 8'd0;
                    vga_y = 8'd0;
                    vga_plot = 1'd0;
                    nextState = `DONER;
                end
                default: begin
                    t_start = 1'd0;
                    l_start = 1'd0;
                    r_start = 1'd0;
                    vga_plot = 1'd0;
                    vga_x = 8'd0;
                    vga_y = 8'd0;
                    nextState = `RESETR;
                end
            endcase
            if (currState == `DONER) begin
                done = 1'd1;
            end else begin
                done = 1'd0;
            end
        end else begin
            t_start = 1'd0;
            l_start = 1'd0;
            r_start = 1'd0;
            vga_plot = 1'd0;
            vga_x = 8'd0;
            vga_y = 8'd0;
            done = 1'd0;
            nextState = `RESETR;
        end
    end
endmodule

