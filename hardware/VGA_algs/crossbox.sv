`define RESET 3'd0
`define WAIT_FOR_PLAYER 3'd1
`define GET_INPUTS 3'd2
`define PAINT_SQUARE_RIGHT 3'd3
`define PAINT_SQUARE_LEFT 3'd4
`define DONE 3'd5

`define BASEX_LEFT 9'd10
`define BASEX_RIGHT 9'd178
`define BASEY 8'd91
`define SQUARESIZE 8'd12

module crossbox(input logic clk, input logic rst_n, input logic [2:0] colour,
                  input logic start, input [3:0] x_square, input [3:0] y_square, 
                  input player, output logic done,
                  output logic [8:0] vga_x, output logic [7:0] vga_y,
                  output logic [2:0] vga_colour, output logic vga_plot);

    logic [8:0] current_x;
    logic [8:0] the_x_im_looking_for;
    logic [8:0] curr_base_x;
    logic [7:0] current_y;
    logic [7:0] curr_base_y;

    logic [2:0] currState;
    logic [2:0] nextState;

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
            current_x <= 9'd0;
            curr_base_x <= 9'd0;
            curr_base_y <= 8'd90;
            current_y <= 8'd90;         
        end
        else if (currState == `GET_INPUTS) begin
            current_x <= player ? `BASEX_RIGHT + (9'd14 * (x_square % 9'd10)) : `BASEX_LEFT + (9'd14 * (x_square % 9'd10));
            curr_base_x <= player ? `BASEX_RIGHT + (9'd14 * (x_square % 9'd10)) : `BASEX_LEFT + (9'd14 * (x_square % 9'd10));
            current_y <= `BASEY + 8'd14 * (y_square % 8'd10);
            curr_base_y <= `BASEY + 8'd14 * (y_square % 8'd10);
        end 
        else if (currState == `PAINT_SQUARE_RIGHT) begin
            if (current_y <= `BASEY + 8'd14 * (y_square % 8'd10) + (`SQUARESIZE - 8'd2)) begin
                current_x <= current_x + 9'd1;
                current_y <= current_y + 9'd1;
            end
        end
        else if (currState == `PAINT_SQUARE_LEFT) begin
            if (current_y < `BASEY + 8'd14 * (y_square % 8'd10) + (`SQUARESIZE - 8'd1)) begin
                current_x <= current_x - 9'd1;
                current_y <= current_y + 9'd1;
            end else if (current_x >= curr_base_x + `SQUARESIZE - 9'd2) begin
                current_y <= curr_base_y;
            end
        end
        else begin
            current_x <= 9'd0;
            current_y <= 8'd90; 
        end
    end

    always_comb begin
        if (start) begin
            case(currState)
                `RESET: begin
                    vga_plot = 1'd0;
                    vga_x = 9'd0;
                    vga_y = 8'd0;
                    nextState = `GET_INPUTS;
                end
                `GET_INPUTS: begin
                    vga_plot = 1'd0;
                    vga_x = 9'd0;
                    vga_y = 8'd0;
                    nextState = `PAINT_SQUARE_RIGHT;
                end
                `PAINT_SQUARE_RIGHT: begin
                    vga_x = current_x;
                    vga_y = current_y;
                    vga_plot = 1'd1;
                    
                    if (current_y >= `BASEY + 8'd14 * (y_square % 8'd10) + (`SQUARESIZE - 8'd1)) begin
                        nextState = `PAINT_SQUARE_LEFT;
                    end else begin
                        nextState = `PAINT_SQUARE_RIGHT;
                    end
                end
                `PAINT_SQUARE_LEFT: begin
                    vga_x = current_x;
                    vga_y = current_y;
                    
                    if ((current_y >= curr_base_y) && (current_x <= curr_base_x)) begin
                        nextState = `DONE;
                        vga_plot = 1'd1;
                    end else begin
                        nextState = `PAINT_SQUARE_LEFT;
                        vga_plot = 1'd1;
                    end
                end
                `DONE: begin
                    vga_x = 9'd0;
                    vga_y = 8'd0;
                    vga_plot = 1'd0;
                    nextState = `DONE;
                end
                default: begin
                    vga_x = 9'd0;
                    vga_y = 8'd0;
                    vga_plot = 1'd0;
                    nextState = `RESET;
                end
            endcase
            if (currState == `DONE) begin
                done = 1'd1;
            end else begin
                done = 1'd0;
            end
        end else begin
            vga_plot = 1'd0;
            vga_x = 9'd0;
            vga_y = 8'd0;
            done = 1'd0;
            nextState = `RESET;
        end
    end

    assign vga_colour = 3'd100;
endmodule