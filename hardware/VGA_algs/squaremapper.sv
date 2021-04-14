`define RESET 3'd0
`define WAIT_FOR_PLAYER 3'd1
`define GET_INPUTS 3'd2
`define PAINT_SQUARE 3'd3
`define DONE 3'd4

`define BASEX_LEFT 9'd10
`define BASEX_RIGHT 9'd178
`define BASEY 8'd91

`define MISS 3'd0
`define HIT 3'd1
`define SUNK 3'd2

module squaremapper(input logic clk, input logic rst_n, input logic [2:0] colour,
                  input logic start, input [3:0] x_square, input [3:0] y_square, 
                  input player, input [1:0] attack_type, output logic done,
                  output logic [8:0] vga_x, output logic [7:0] vga_y,
                  output logic [2:0] vga_colour, output logic vga_plot);

    logic [8:0] current_x;
    logic [8:0] the_x_im_looking_for;
    logic [8:0] curr_base_x;
    logic [7:0] current_y;

    logic [2:0] currState;
    logic [2:0] nextState;

    assign curr_base_x = player ? `BASEX_RIGHT : `BASEX_LEFT;

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
            current_y <= 8'd90;         
        end
        else if (currState == `GET_INPUTS) begin
            current_x <= player ? `BASEX_RIGHT + (9'd14 * (x_square % 9'd10)) : `BASEX_LEFT + (9'd14 * (x_square % 9'd10));
            current_y <= `BASEY + 8'd14 * (y_square % 8'd10);
        end 
        else if (currState == `PAINT_SQUARE) begin
            if (current_y >= `BASEY + (8'd14 * (y_square + 8'd1)) - 8'd3) begin
                current_x <= current_x + 9'd1;
                current_y <= `BASEY + 8'd14 * y_square;
            end 
            else begin
                current_y <= current_y + 8'd1;
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
                    nextState = `PAINT_SQUARE;
                end
                `PAINT_SQUARE: begin
                    vga_x = current_x;
                    vga_y = current_y;
                    
                    if (current_x >= (curr_base_x + (9'd14 * (x_square + 9'd1) - 9'd2))) begin
                        vga_plot = 1'd0;
                        nextState = `DONE;
                    end else begin
                        vga_plot = 1'd1;
                        nextState = `PAINT_SQUARE;
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

    always_comb begin
        case (attack_type)
            `MISS: begin
                vga_colour = 3'b111;
            end
            `HIT: begin
                vga_colour = 3'b100;
            end
            `SUNK: begin
                vga_colour = 3'b101;
            end
            default: begin
                vga_colour = 3'b110;
            end
        endcase
    end
endmodule

