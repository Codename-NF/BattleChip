module blankboard(input logic clk, input logic rst_n, input logic [2:0] colour,
                  input logic start, output logic done,
                  output logic [8:0] vga_x, output logic [7:0] vga_y,
                  output logic [2:0] vga_colour, output logic vga_plot);

    logic [8:0] current_x;
    logic [7:0] current_y;

    assign vga_x = current_x;
    assign vga_y = current_y;

    always @(posedge clk or negedge rst_n) begin
        if (rst_n == 1'b0) begin
            current_x <= 9'd10;
            current_y <= 8'd90;
            done <= 1'd0;
            vga_plot <= 1'd0;
        end
        else begin
            if (start == 1'b1 && done == 1'b0) begin
                if (vga_plot == 1'd0) begin
                    vga_plot <= 1'd1;
                end else if (current_x === 9'd317 && current_y === 8'd239) begin
                    current_x <= 9'd10;
                    current_y <= 8'd90;
                    done <= 1'd1;
                    vga_plot <= 1'd0;
                end else if (current_y === 8'd239) begin
                    current_y <= 8'd90;
                    current_x <= current_x + 9'd1;
                    vga_plot <= 1'd1;
                end else begin
                    current_y <= current_y + 7'd1;
                    vga_plot <= 1'd1;
                end
            end else if (start == 1'b0) begin
                current_x <= 9'd10;
                current_y <= 8'd90;
                done <= 1'd0;
                vga_plot <= 1'd0;
            end
        end
    end

    always_comb begin
        if (current_x > 9'd148 && current_x < 9'd176) begin
            vga_colour = 3'd0;
        end else if (current_x > 9'd316) begin
            vga_colour = 3'd0;
        end else if (current_x < 9'd4) begin
            vga_colour = 3'd0;
        end else if (current_y > 8'd230) begin
            vga_colour = 3'd0;
        end else if ((current_x % 9'd14) === 8) begin
            vga_colour = 3'd0;
        end else if ((current_x % 9'd14) === 9) begin
            vga_colour = 3'd0;
        end else if ((current_y % 9'd14) === 5) begin
            vga_colour = 3'd0;
        end else if ((current_y % 9'd14) === 6) begin
            vga_colour = 3'd0;
        end else begin
            vga_colour = 3'd1;
        end
        
    end
endmodule

