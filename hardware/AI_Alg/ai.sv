`define set_density 3'd0
`define clean_var 3'd1
`define check_fire 3'd2
`define add_density 3'd3
`define waiting 3'd4

module ai(input logic [99:0] fired, input logic [4:0] ships, input logic clk, input logic rst_n, output logic [99:0][5:0] density
            output logic done);

    reg [1:0] ship0_density_x, ship0_density_y;
    reg [2:0][1:0] ship1_2_density_x, ship1_2_density_y;
    reg [2:0] state;
    reg [3:0] ship3_density_x, ship3_density_y, x, y;
    reg [4:0] ship4_density_x, ship4_density_y;
    reg [6:0] pos, largest_index;

	always @(posedge clk or negedge rst_n) begin
        if (rst_n === 1'b0) begin
            state <= `set_density;
            done <= 1'd0;
            pos <= 7'd0;
        end   
        else begin
            case (state)
                `set_density: begin
                    density[pos] <= 6'd0;
                    density[pos + 1] <= 6'd0;
                    density[pos + 2] <= 6'd0;
                    density[pos + 3] <= 6'd0;
                    density[pos + 4] <= 6'd0;
                    if (pos === 7'd95) begin
                        state <= `waiting;
                        pos <= 7'd0;
                        done <= 1'b1;
                    end
                    else begin
                        pos <= pos + 7'd5;
                    end
                end
                `clean_var: begin
                    state <= `check_fire;
                    x <= pos % 10;
                    y <= pos / 10;
                    ship0_density_x <= 2'd0;
                    ship1_2_density_x <= 6'd0;
                    ship3_density_x <= 4'd0;
                    ship4_density_x <= 5'd0;
                    ship0_density_y <= 2'd0;
                    ship1_2_density_y <= 6'd0;
                    ship3_density_y <= 4'd0;
                    ship4_density_y <= 5'd0;
                end
                `check_fire: begin
                    state <= `add_density;
                    if (ships[0] && x < 4'd9) begin
                        if (fired[pos] === 1'd0 && fired[pos + 1] === 1'd0) begin
                            ship0_density_x[0] <= 1'd1;
                            ship0_density_x[1] <= 1'd1;
                        end
                    end
                    if (ships[0] && (y < 4'd9)) begin
                        if (fired[pos] === 1'd0 && fired[pos + 10] === 1'd0) begin
                            ship0_density_y[0] <= 1'd1;
                            ship0_density_y[1] <= 1'd1;
                        end	
                    end
                    if ((ships[1] || ships[2]) && (x < 4'd8)) begin
                        if (fired[pos] === 1'd0 && fired[pos + 1] === 1'd0 && fired[pos + 2] === 1'd0) begin
                            ship1_2_density_x[0] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                            ship1_2_density_x[1] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                            ship1_2_density_x[2] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                        end     
                    end  
                    if ((ships[1] || ships[2]) && (y < 4'd8)) begin
                        if (fired[pos] === 1'd0 && fired[pos + 10] === 1'd0 && fired[pos + 20] === 1'd0) begin
                            ship1_2_density_y[0] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                            ship1_2_density_y[1] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                            ship1_2_density_y[2] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                        end     
                    end                        
                    if (ships[3] && (x < 4'd7)) begin
                        if (fired[pos] === 1'd0 && fired[pos + 1] === 1'd0 && fired[pos + 2] === 1'd0 && fired[pos + 3] === 1'd0) begin
                            ship3_density_x[0] <= 1'd1;
                            ship3_density_x[1] <= 1'd1;
                            ship3_density_x[2] <= 1'd1;
                            ship3_density_x[3] <= 1'd1;
                        end
                    end
                    if (ships[3] && (y < 4'd7)) begin
                        if (fired[pos] === 1'd0 && fired[pos + 10] === 1'd0 && fired[pos + 20] === 1'd0 && fired[pos + 30] === 1'd0) begin
                            ship3_density_y[0] <= 1'd1;
                            ship3_density_y[1] <= 1'd1;
                            ship3_density_y[2] <= 1'd1;
                            ship3_density_y[3] <= 1'd1;
                        end
                    end       
                    if (ships[4] && (x < 4'd6)) begin
                        if (fired[pos] === 1'd0 && fired[pos+ 1] === 1'd0 && fired[pos+ 2] === 1'd0 && fired[pos+ 3] === 1'd0 && fired[pos+ 4] === 1'd0) begin
                            ship4_density_x[0] <= 1'd1;
                            ship4_density_x[1] <= 1'd1;
                            ship4_density_x[2] <= 1'd1;
                            ship4_density_x[3] <= 1'd1;
                            ship4_density_x[4] <= 1'd1;
                        end    
                    end
                    if (ships[4] && (y < 4'd6)) begin
                        if (fired[pos] === 1'd0 && fired[pos+ 10] === 1'd0 && fired[pos+ 20] === 1'd0 && fired[pos+ 30] === 1'd0 && fired[pos+ 40] === 1'd0) begin
                            ship4_density_y[0] <= 1'd1;
                            ship4_density_y[1] <= 1'd1;
                            ship4_density_y[2] <= 1'd1;
                            ship4_density_y[3] <= 1'd1;
                            ship4_density_y[4] <= 1'd1;
                        end    
                    end
                end
                `add_density: begin
                    if (pos + 7'd1 === 7'd99) begin
                        state <= `find_largest_density;
                        pos <= 7'd1;
                        largest_index <= 7'd0;
                    end
                    else begin
                        state <= `clean_var;
                        pos <= pos + 7'd1;
                    end
                    if (x < 6) begin
                        density[pos + 4] <= density[pos + 4] + ship4_density_x[4];
                    end
                    if (x < 7) begin
                        density[pos + 3] <= density[pos + 3] + ship3_density_x[3] + ship4_density_x[3];
                    end
                    if (x < 8) begin
                        density[pos + 2] <= density[pos + 2] + ship1_2_density_x[2] + ship3_density_x[2] + ship4_density_x[2];
                    end
                    if (x < 9) begin
                        density[pos + 1] <= density[pos + 1] + ship0_density_x[1] + ship1_2_density_x[1] + ship3_density_x[1] + ship4_density_x[1];
                    end
                    density[pos] <= density[pos] + ship0_density_x[0] + ship1_2_density_x[0] + ship3_density_x[0] + ship4_density_x[0]
                                                + ship0_density_y[0] + ship1_2_density_y[0] + ship3_density_y[0] + ship4_density_y[0];
                    if (y < 9) begin
                        density[pos + 10] <= density[pos + 10] + ship0_density_y[1] + ship1_2_density_y[1] + ship3_density_y[1] + ship4_density_y[1];
                    end
                    if (y < 8) begin
                        density[pos + 20] <= density[pos + 20] + ship1_2_density_y[2] + ship3_density_y[2] + ship4_density_y[2];
                    end
                    if (y < 7) begin
                        density[pos + 30] <= density[pos + 30] + ship3_density_y[3] + ship4_density_y[3];
                    end
                    if (y < 6) begin
                        density[pos + 40] <= density[pos + 40] + ship4_density_y[4];
                    end
                end
                `find_largest_density: begin
                    if (pos === 7'd99) begin
                        done <= 1'd1;
                        state <= `set_density;
                    end
                    if (density[pos] > density[largest_index]) begin
                        largest_index <= pos;
                    end
                    pos <= pos + 7'd1;
                end
                default: begin
                    if (start <= 1'd1) begin
                        done <=  1'd0;
                        state <= clean_var;
                    end
                    else begin
                        done <= 1'd1;
                    end
                end
            endcase
        end         
	end
endmodule 