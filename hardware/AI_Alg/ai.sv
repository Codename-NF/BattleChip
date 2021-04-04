`define set_density 3'd0
`define clean_var 3'd1
`define check_fire 3'd2
`define add_density 3'd3
`define find_largest_density 3'd4
`define waiting 3'd5

// module ai(input logic [99:0] fired, input logic [99:0] hits, input logic [4:0] ships, input logic clock, input logic reset_n,
//             output logic [6:0] largest_index, output logic wait_request, input logic start);

module ai(input clock, input reset_n, input unsigned [3:0] addr, input write_en, input read_en, input logic [31:0] data_in,
             output logic wait_request, output logic [31:0] data_out);

    reg [99:0] fired, hits;
    reg [6:0] largest_index;
    reg [4:0] ships;

    reg hitmode;
    reg [1:0] ship0_density_x, ship0_density_y;
    reg [2:0][1:0] ship1_2_density_x, ship1_2_density_y;
    reg [2:0] state;
    reg [3:0] ship3_density_x, ship3_density_y, x, y;
    reg [4:0] ship4_density_x, ship4_density_y;
    reg [6:0] pos;
    reg [99:0][5:0] density;

    // assign data_out = 7'd69;

	always @(posedge clock or negedge reset_n) begin
        if (reset_n === 1'b0) begin
            state <= `set_density;
            wait_request <= 1'd1;
            pos <= 7'd0;
        end   
        else begin
            case (state)
                // Set all the values in the density wire to be 0
                `set_density: begin
                    density[pos] <= 6'd0;
                    density[pos + 1] <= 6'd0;
                    density[pos + 2] <= 6'd0;
                    density[pos + 3] <= 6'd0;
                    density[pos + 4] <= 6'd0;
                    if (pos === 7'd95) begin
                        state <= `waiting;
                        pos <= 7'd0;
                        wait_request <= 1'b0;
                    end
                    else begin
                        pos <= pos + 7'd5;
                        wait_request <= 1'b1;
                    end
                end
                // Reset all variables 
                `clean_var: begin
                    state <= `check_fire;
                    x <= pos % 4'd10;
                    y <= pos / 4'd10;
                    hitmode <= |hits;
                    ship0_density_x <= 2'd0;
                    ship1_2_density_x <= 6'd0;
                    ship3_density_x <= 4'd0;
                    ship4_density_x <= 5'd0;
                    ship0_density_y <= 2'd0;
                    ship1_2_density_y <= 6'd0;
                    ship3_density_y <= 4'd0;
                    ship4_density_y <= 5'd0;
                end
                // Check horizontal and vertical placements of a ship at a given position
                `check_fire: begin
                    state <= `add_density;
                    // Check if the ship is not sunk, and if it will fit on the horizontal
                    if (ships[0] && x < 4'd9) begin
                        // Check for any fired shots at the placemet and either there are no active hits
                        // or there are active hits and the placement has at least one hit on it
                        if ((fired[pos] === 1'd0 && fired[pos + 1] === 1'd0) && (!hitmode || (hitmode &&
                                (hits[pos] === 1'd1 || hits[pos + 1] === 1'd1)))) begin
                            ship0_density_x[0] <= 1'd1;
                            ship0_density_x[1] <= 1'd1;
                        end
                    end
                    if (ships[0] && (y < 4'd9)) begin
                        if ((fired[pos] === 1'd0 && fired[pos + 10] === 1'd0) && (!hitmode || (hitmode &&
                                (hits[pos] === 1'd1 || hits[pos + 10] === 1'd1)))) begin
                            ship0_density_y[0] <= 1'd1;
                            ship0_density_y[1] <= 1'd1;
                        end	
                    end
                    if ((ships[1] || ships[2]) && (x < 4'd8)) begin
                        if ((fired[pos] === 1'd0 && fired[pos + 1] === 1'd0 && fired[pos + 2] === 1'd0) &&
                                (!hitmode || (hitmode && 
                                (hits[pos] === 1'd1 || hits[pos + 1] === 1'd1 || hits[pos + 2] === 1'd1)))) begin
                            // Small trick to check both 3 length ships at once
                            ship1_2_density_x[0] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                            ship1_2_density_x[1] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                            ship1_2_density_x[2] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                        end     
                    end  
                    if ((ships[1] || ships[2]) && (y < 4'd8)) begin
                        if ((fired[pos] === 1'd0 && fired[pos + 10] === 1'd0 && fired[pos + 20] === 1'd0) &&
                                (!hitmode || (hitmode && 
                                (hits[pos] === 1'd1 || hits[pos + 10] === 1'd1 || hits[pos + 20] === 1'd1)))) begin
                            ship1_2_density_y[0] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                            ship1_2_density_y[1] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                            ship1_2_density_y[2] <= (2'd1 && ships[1])  +  (2'd1 && ships[2]);
                        end     
                    end                        
                    if (ships[3] && (x < 4'd7)) begin
                        if ((fired[pos] === 1'd0 && fired[pos + 1] === 1'd0 && fired[pos + 2] === 1'd0 && fired[pos + 3] === 1'd0) &&
                                (!hitmode || (hitmode && (hits[pos] === 1'd1 || hits[pos + 1] === 1'd1 ||
                                hits[pos + 2] === 1'd1 || hits[pos + 3] === 1'd1)))) begin
                            ship3_density_x[0] <= 1'd1;
                            ship3_density_x[1] <= 1'd1;
                            ship3_density_x[2] <= 1'd1;
                            ship3_density_x[3] <= 1'd1;
                        end
                    end
                    if (ships[3] && (y < 4'd7)) begin
                        if ((fired[pos] === 1'd0 && fired[pos + 10] === 1'd0 && fired[pos + 20] === 1'd0 &&
                                fired[pos + 30] === 1'd0) && (!hitmode || (hitmode && (hits[pos] === 1'd1 ||
                                hits[pos + 10] === 1'd1 || hits[pos + 20] === 1'd1 || hits[pos + 30] === 1'd1)))) begin
                            ship3_density_y[0] <= 1'd1;
                            ship3_density_y[1] <= 1'd1;
                            ship3_density_y[2] <= 1'd1;
                            ship3_density_y[3] <= 1'd1;
                        end
                    end       
                    if (ships[4] && (x < 4'd6)) begin
                        if ((fired[pos] === 1'd0 && fired[pos+ 1] === 1'd0 && fired[pos+ 2] === 1'd0 &&
                                fired[pos+ 3] === 1'd0 && fired[pos+ 4] === 1'd0) && (!hitmode || (hitmode &&
                                (hits[pos] === 1'd1 || hits[pos + 1] === 1'd1 || hits[pos + 2] === 1'd1 ||
                                hits[pos + 3] === 1'd1 || hits[pos + 4] === 1'd1)))) begin
                            ship4_density_x[0] <= 1'd1;
                            ship4_density_x[1] <= 1'd1;
                            ship4_density_x[2] <= 1'd1;
                            ship4_density_x[3] <= 1'd1;
                            ship4_density_x[4] <= 1'd1;
                        end    
                    end
                    if (ships[4] && (y < 4'd6)) begin
                        if ((fired[pos] === 1'd0 && fired[pos+ 10] === 1'd0 && fired[pos+ 20] === 1'd0 &&
                                fired[pos+ 30] === 1'd0 && fired[pos+ 40] === 1'd0) && (!hitmode || (hitmode &&
                                (hits[pos] === 1'd1 || hits[pos + 10] === 1'd1 || hits[pos + 20] === 1'd1 ||
                                hits[pos + 30] === 1'd1 || hits[pos + 40] === 1'd1)))) begin
                            ship4_density_y[0] <= 1'd1;
                            ship4_density_y[1] <= 1'd1;
                            ship4_density_y[2] <= 1'd1;
                            ship4_density_y[3] <= 1'd1;
                            ship4_density_y[4] <= 1'd1;
                        end    
                    end
                end
                // Add all the weights caculated from the postion to the wire
                `add_density: begin
                    // Check if the position is at the end
                    if (pos + 7'd1 === 7'd99) begin
                        state <= `find_largest_density;
                        pos <= 7'd1;
                        largest_index <= 7'd0;
                    end
                    else begin
                        state <= `clean_var;
                        pos <= pos + 7'd1;
                    end
                    // Check if there is room to add the ship (don't overflow into the next row)
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
                    // Current position is always valid
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
                // Look through all densities and find the largest one at the lowest position
                `find_largest_density: begin
                    if (pos === 7'd99) begin
                        state <= `set_density;
                        pos <= 7'd0;
                        data_out <= largest_index;
                    end
                    else begin
                        pos <= pos + 7'd1;
                    end
                    // Check if current pos is higher, and don't count positions at hits
                    if ((density[pos] & {6{~hits[pos]}}) > (density[largest_index] & {6{~hits[pos]}})) begin
                        largest_index <= pos;
                    end
                end
                // Waiting state
                default: begin
                    if (write_en === 1'd1) begin
                        if (addr === 4'd0) begin
                            wait_request <=  1'd1;
                            state <= `clean_var;
                        end
                        else if (addr === 4'd1) begin
                            fired[31:0] <= data_in[31:0];
                        end
                        else if (addr === 4'd2) begin
                            fired[63:32] <= data_in[31:0];
                        end
                        else if (addr === 4'd3) begin
                            fired[95:64] <= data_in[31:0];
                        end
                        else if (addr === 4'd4) begin
                            fired[99:96] <= data_in[3:0];
                        end
                        else if (addr === 4'd5) begin
                            hits[31:0] <= data_in[31:0];
                        end
                        else if (addr === 4'd6) begin
                            hits[63:32] <= data_in[31:0];
                        end
                        else if (addr === 4'd7) begin
                            hits[95:64] <= data_in[31:0];
                        end
                        else if (addr === 4'd8) begin
                            hits[99:96] <= data_in[3:0];
                        end
                        else if (addr === 4'd9) begin
                            ships <= data_in[4:0];
                        end
                    end
                    if (read_en === 1'd1) begin
                        if (addr === 4'd0) begin
                            data_out <= largest_index;
                        end
                        else if (addr === 4'd1) begin
                            data_out <= fired[31:0];
                        end
                        else if (addr === 4'd2) begin
                            data_out <= fired[63:32];
                        end
                        else if (addr === 4'd3) begin
                            data_out <= fired[95:64];
                        end
                        else if (addr === 4'd4) begin
                            data_out <= {28'd0,fired[99:96]};
                        end
                        else if (addr === 4'd5) begin
                            data_out <= hits[31:0];
                        end
                        else if (addr === 4'd6) begin
                            data_out <= hits[63:32];
                        end
                        else if (addr === 4'd7) begin
                            data_out <= hits[95:64];
                        end
                        else if (addr === 4'd8) begin
                            data_out <= {28'd0,hits[99:96]};
                        end
                        else if (addr === 4'd9) begin
                            data_out <= {27'd0,ships[4:0]};
                        end
                    end
                    // if (read_en === 1'd1) begin
                    //     data_out <= largest_index;
                    // end
                end
            endcase
        end         
	end
endmodule 