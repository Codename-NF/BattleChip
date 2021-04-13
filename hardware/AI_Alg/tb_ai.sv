module tb_ai();

    reg clock, reset_n, write_en, read_en;
    reg [3:0] addr;
    reg [31:0] data_in;
    wire wait_request;
    wire [31:0] data_out;

    ai DUT(clock, reset_n, addr, write_en, read_en, data_in, wait_request, data_out);

    initial begin
        clock = 1'b1;
        forever begin
            clock = ~clock;
            #50;
        end
    end

    initial begin
        reset_n = 1'd0;
        #100;
        reset_n = 1'd1;
        wait (wait_request === 1'd0);
        #50;
        write_en = 1'd1;
        read_en = 1'd0;
        addr = 4'd1;
        data_in = 32'd0; // Set fire addr(1,2,3,4) and hits addr(5,6,7,8) to 0
        #100;
        addr = 4'd2;
        data_in = 32'd0;
        #100;
        addr = 4'd3;
        data_in = 32'd0;
        #100;
        addr = 4'd4;
        data_in = 32'd0;
        #100;
        addr = 4'd5;
        data_in = 32'd0;
        #100;
        addr = 4'd6;
        data_in = 32'd0;
        #100;
        addr = 4'd7;
        data_in = 32'd0;
        #100;
        addr = 4'd8;
        data_in = 32'd0;
        #100;
        addr = 4'd9;
        data_in = 32'd31; // set ships addr(9) to b11111 = d31
        #100;
        addr = 4'd0; // Start command
        data_in = 32'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0); // Wait until Ai says it's done
        #50;
        read_en = 1'd1; // Read data at addr 0, which is the place to shoot
        #100;
        assert(data_out === 32'd44); // It should have shot at 44
        read_en = 1'd0;
        write_en = 1'd1;
        addr = 4'd2;
        data_in = 32'd4096; // Set fired index 44 to 1
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0);
        #50;
        read_en = 1'd1;
        #100;
        assert(data_out === 32'd55); // Hit battleship at 55
        read_en = 1'd0;
        write_en = 1'd1;
        addr = 4'd6;
        data_in = 32'd8388608; // Add hit at 55
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0);
        #50;
        read_en = 1'd1;
        #100;
        assert(data_out === 32'd45); // Miss at 45
        read_en = 1'd0;
        write_en = 1'd1;
        addr = 4'd2;
        data_in = 32'd24576; // Fired at 44, 45
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0);
        #50;
        read_en = 1'd1;
        #100;
        assert(data_out === 32'd54); // Hit battleship at 54
        read_en = 1'd0;
        write_en = 1'd1;
        addr = 4'd6;
        data_in = 32'd12582912; // Hits at 55, 54
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0); 
        #50;
        read_en = 1'd1;
        #100;
        assert(data_out === 32'd53); // Hit battleship at 53
        read_en = 1'd0;
        write_en = 1'd1;
        addr = 4'd6;
        data_in = 32'd14680064; // Hits at 55, 54, 53
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0); 
        #50;
        read_en = 1'd1;
        #100;
        assert(data_out === 32'd56); // Sunk battleship at 56
        read_en = 1'd0;
        write_en = 1'd1;
        addr = 4'd6;
        data_in = 32'd0; // No more active hits
        #100;
        addr = 4'd2;
        data_in = 32'd31469568; // Add sunk shots to fired. Fired at 44,45,53,54,55,56
        #100;
        addr = 4'd9;
        data_in = 32'd23; // Remove the battleship from the active ships
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0); 
        #50;
        read_en = 1'd1;
        #100;
        assert(data_out === 32'd32); // Miss at 32
        read_en = 1'd0;
        write_en = 1'd1;
        addr = 4'd2;
        data_in = 32'd31469569; // Fired at 32,44,45,53,54,55,56
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0); 
        #50;
        read_en = 1'd1;
        #100;
        assert(data_out === 32'd37); // Hit submarine at 37
        read_en = 1'd0;
        write_en = 1'd1;
        addr = 4'd6;
        data_in = 32'd32; // Hit at 37
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0); 
        #50;
        read_en = 1'd1;
        #100;
        assert(data_out === 32'd47); // Miss at 47
        read_en = 1'd0;
        write_en = 1'd1;
        addr = 4'd2;
        data_in = 32'd31502337; // Fired at 32,44,45,47,53,54,55,56
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0); 
        #50;
        read_en = 1'd1;
        #100;
        assert(data_out === 32'd36); // Hit at 36
        read_en = 1'd0;
        write_en = 1'd1;
        addr = 4'd6;
        data_in = 32'd48; // Hits at 36,37
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0); 
        #50;
        read_en = 1'd1;
        #100;
        assert(data_out === 32'd35); // Miss at 35
        read_en = 1'd0;
        write_en = 1'd1;
        addr = 4'd2;
        data_in = 32'd31502345; // Fired at 32,35,44,45,47,53,54,55,56
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0); 
        #50;
        read_en = 1'd1;
        #100;
        assert(data_out === 32'd38); // Sunk submarine at 38
        read_en = 1'd0;
        write_en = 1'd1;
        addr = 4'd6;
        data_in = 32'd0; // No more active hits
        #100;
        addr = 4'd2;
        data_in = 32'd31502457; // Add sunk shots to fired. Fired at 32,35,36,37,38,44,45,47,53,54,55,56
        #100;
        addr = 4'd9;
        data_in = 32'd21; // Remove the submarine from the active ships
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0); 
        #50;
        read_en = 1'd1;
        #100;
        assert(data_out === 32'd38); // Miss at 23 // Ending the game here since it's more of the same stuff
        // Original testbench tested a full game and can be seen in tb_ai_pre_avalon
        // Ai chooses slightly different positions with that game because multipe hit weighting wasn't taken into account
        #100;
        $stop;
    end
endmodule: tb_ai