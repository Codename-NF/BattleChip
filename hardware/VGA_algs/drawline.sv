module drawline(
        input logic clk, input logic rst_n,
        input logic [2:0] colour,
        input logic [8:0] x0, input logic [7:0] y0,
        input logic [8:0] x1, input logic [7:0] y1,
        input logic start, output logic done,
        output logic [8:0] vga_x, output logic [7:0] vga_y,
        output logic [2:0] vga_colour, output logic vga_plot
    );

	logic signed [9:0] dx;
	logic signed [9:0] dy;
	logic signed [1:0] sx;
	logic signed [1:0] sy;
	logic signed [9:0] err;
	logic signed [10:0] e2;
	logic init;
	logic first;
	assign dx = (x1 > x0) ? x1 - x0 : x0 - x1;
	assign dy = (y1 > y0) ? y0 - y1 : y1 - y0;
	assign sx = (x0 < x1) ? 2'sd1 : 2'sd0 - 2'sd1;
	assign sy = (y0 < y1) ? 2'sd1 : 2'sd0 - 2'sd1;

	enum {RESET, COMPUTE, DONE} nextState, currState;

	assign vga_colour = colour;

	logic [7:0] curr_x;
	logic [7:0] curr_y;

	always @(posedge clk) begin
		if (rst_n == 1'd0) begin
			done <= 1'd0;
			curr_x <= 1'd0;
			curr_y <= 1'd0;
			init <= 1'd1;
			err <= 2'd0;
			vga_plot <= 1'd0;
		end 
		else if (start == 1'd1) begin
			if (init == 1'd1) begin
				init <= 1'd0;
				curr_x <= x0;
				curr_y <= y0;
				err <= dx + dy;
				first <= 1'd0;
			end else begin
				if (first == 1'd1) begin
					first <= 1'd0;
					done <= 1'd1;
				end
				else if (curr_x < 10'sd320 && curr_x >= 9'sd0 && curr_y < 10'sd240 && curr_y >= 9'sd0 && done == 1'd0) begin
					vga_plot <= 1'd1;
					vga_x <= curr_x;
					vga_y <= curr_y;
				end
				if (curr_x == {1'sd0, x1} && curr_y == {1'sd0, y1} && done == 1'd0) begin
					first <= 1'd1;
				end else begin
					if (2 * err >= dy && 2* err <= dx) begin
						err <= err + dx + dy;
						if (sx < 0) begin 
							curr_x <= curr_x - 1;
						end else begin
							curr_x <= curr_x + 1;
						end
						if (sy < 0) begin
							curr_y <= curr_y - 1;
						end else begin
							curr_y <= curr_y + 1;
						end
						
					end else if (2 * err >= dy) begin
						err <= err + dy;
						if (sx < 0) begin 
							curr_x <= curr_x - 1;
						end else begin
							curr_x <= curr_x + 1;
						end
					end else if (2 * err <= dx) begin
						err <= err + dx;
						if (sy < 0) begin
							curr_y <= curr_y - 1;
						end else begin
							curr_y <= curr_y + 1;
						end
					end
				end
			end
		end else begin
			done <= 1'd0;
			curr_x <= x0;
			curr_y <= y0;
			err <= dx + dy;
			init = 1'd1;
			vga_plot <= 1'd0;
		end
	end





endmodule
