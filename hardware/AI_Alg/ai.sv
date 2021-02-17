module ai(input logic [99:0] fired, input logic [4:0] ships, input logic rst_n, output logic [99:0][5:0] density);

    reg [1:0] ship0_density;
    reg [2:0][1:0] ship1_2_density;
    reg [3:0] ship3_density, x;
    reg [4:0] ship4_density;
    reg [6:0] pos;

	always @(posedge clk or negedge rst_n) begin
        if (rst_n === 1'b0) begin
            state <= `state#;
            done <= 1'b0;
            pos <= 7'd0;
        end   
        else begin
            case (state)
                `state#: begin
                    density[pos] <= 6'd0;
                    density[pos + 1] <= 6'd0;
                    density[pos + 2] <= 6'd0;
                    density[pos + 3] <= 6'd0;
                    density[pos + 4] <= 6'd0;
                    if (pos === 7'd95) begin
                        state <= `state1;
                        pos <= 7'd0;
                    end
                    else begin
                        pos <= pos + 7'd5;
                    end
                end
                `state1: begin
                    x <= pos % 10;
                    ship0_density <= 2'd0;
                    ship1_2_density <= 6'd0;
                    ship3_density <= 4'd0;
                    ship4_density <= 5'd0;
                end
                `state2: begin
                    if ships[0] && x < 4'd9 begin
                        if fired[pos] === 1'd0 && fired[pos + 1] === 1'd0 begin
                            ship0_density[0] <= 1'd1;
                            ship0_density[1] <= 1'd1;
                        end
                        else begin
                            ship0_density[0] <= 1'd0;
                            ship0_density[1] <= 1'd0;
                        end		
                    end
                    if (ships[1] || ships[2]) && x < 4'd8 begin
                        if fired[pos] === 1'd0 && fired[pos + 1] === 1'd0 && fired[pos + 2] === 1'd0 begin
                            ship1_2_density[0] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                            ship1_2_density[1] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                            ship1_2_density[2] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                        end
                        else begin
                            ship1_2_density[0] <= 2'd0;
                            ship1_2_density[1] <= 2'd0;
                            ship1_2_density[2] <= 2'd0;
                        end         
                    end                        
                    if ships[3] && x < 4'd7 begin
                        if fired[pos] === 1'd0 && fired[pos + 1] === 1'd0 && fired[pos + 2] === 1'd0 && fired[pos + 3] === 1'd0 begin
                            ship3_density[0] <= 1'd1;
                            ship3_density[1] <= 1'd1;
                            ship3_density[2] <= 1'd1;
                            ship3_density[3] <= 1'd1;
                        end
                        else begin
                            ship3_density[0] <= 1'd0;
                            ship3_density[1] <= 1'd0;
                            ship3_density[2] <= 1'd0;
                            ship3_density[3] <= 1'd0;
                        end   
                    end       
                    if ships[4] && x < 4'd6 begin
                        if fired[pos] === 1'd0 && fired[pos+ 1] === 1'd0 && fired[pos+ 2] === 1'd0 && fired[pos+ 3] === 1'd0 && fired[pos+ 4] === 1'd0 begin
                            ship4_density[0] <= 1'd1;
                            ship4_density[1] <= 1'd1;
                            ship4_density[2] <= 1'd1;
                            ship4_density[3] <= 1'd1;
                            ship4_density[4] <= 1'd1;
                        end  
                        else begin
                            ship4_density[0] <= 1'd0;
                            ship4_density[1] <= 1'd0;
                            ship4_density[2] <= 1'd0;
                            ship4_density[3] <= 1'd0;
                            ship4_density[4] <= 1'd0;
                        end   
                    end
                end
                `state3: begin
                    pos <= pos + 7'd1;
                    if x < 6 begin
                        density[pos + 4] <= density[pos + 4] + ship4_density[4];
                    end
                    if x < 7 begin
                        density[pos + 3] <= density[pos + 3] + ship3_density[3] + ship4_density[3];
                    end
                    if x < 8 begin
                        density[pos + 2] <= density[pos + 2] + ship1_2_density[2] + ship3_density[2] + ship4_density[2];
                    end
                    if x < 9 begin
                        density[pos] <= density[pos] + ship0_density[0] + ship1_2_density[0] + ship3_density[0] + ship4_density[0];
                        density[pos + 1] <= density[pos + 1] + ship0_density[1] + ship1_2_density[1] + ship3_density[1] + ship4_density[1];
                    end
                end
            endcase
        end         
	end
endmodule 