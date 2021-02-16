module ai(input logic [99:0] fired, input logic rst_n, output logic [99:0] density);

	reg [4:0] ships;
    reg [1:0] ship0_density;
    reg [2:0] ship1_2_density;
    reg [3:0] ship3_density;
    reg [4:0] ship4_density;

	always @(posedge clk or negedge rst_n) begin
        if (rst_n === 1'b0) begin
            state <= `state_done;
            done <= 1'b0;
        end   
        else begin
            case (state)
                `state1; begin
                    
                end
                `state2: begin
                    if ship[0] && x % 10 < 9 begin
                        if fired[x] === 0 && fired[x + 1] === 0 begin
                            ship0_density[x] <= 1;
                            ship0_density[x + 1] <= 1;
                        end
                        else begin
                            ship0_density[x] <= 0;
                            ship0_density[x + 1] <= 0;
                        end		
                    end
                    if (ship[1] || ship[2]) && x % 10 < 8 begin
                        if fired[x] === 0 && fired[x + 1] === 0 && fired[x + 2] === 0 begin
                            ship1_2_density[x]_3 <= (1 && ship[1])  +  (1 && ship[2]);
                            ship1_2_density[x + 1] <= (1 && ship[1])  +  (1 && ship[2]);
                            ship1_2_density[x + 2] <= (1 && ship[1])  +  (1 && ship[2]);
                        end
                        else begin
                            ship1_2_density[x] <= 0;
                            ship1_2_density[x + 1] <= 0;
                            ship1_2_density[x + 2] <= 0;
                        end         
                    end                        
                    if ship[3] && x % 10 < 7 begin
                        if fired[x] === 0 && fired[x + 1] === 0 && fired[x + 2] === 0 && fired[x + 3] === 0 begin
                            ship3_density[x] <= 1;
                            ship3_density[x + 1] <= 1;
                            ship3_density[x + 2] <= 1;
                            ship3_density[x + 3] <= 1;
                        end
                        else begin
                            ship3_density[x] <= 0;
                            ship3_density[x + 1] <= 0;
                            ship3_density[x + 2] <= 0;
                            ship3_density[x + 3] <= 0;
                        end   
                    end       
                    if ship[4] && x % 10 < 6 begin
                        if x === 0 && x + 1 === 0 && x + 2 === 0 && x + 3 === 0 && x + 4 === 0 begin
                            ship4_density[x] <= 1;
                            ship4_density[x + 1] <= 1;
                            ship4_density[x + 2] <= 1;
                            ship4_density[x + 3] <= 1;
                            ship4_density[x + 4] <= 1;
                        end  
                        else begin
                            ship4_density[x] <= 0;
                            ship4_density[x + 1] <= 0;
                            ship4_density[x + 2] <= 0;
                            ship4_density[x + 3] <= 0;
                            ship4_density[x + 4] <= 0;
                        end   
                    end
                end
            endcase
        end         
	end
endmodule 