module tb_ai_monitor();

    reg clock, reset_n, write_en, read_en;
    reg [3:0] addr;
    reg [31:0] data_in;
    wire wait_request;
    wire [31:0] data_out;

    // ai DUT(fired, ships, clk, rst_n, density, largest_index, done, start);
    ai DUT(clock,reset_n, addr, write_en, read_en, data_in, wait_request, data_out);

    initial begin
        clock = 1'b1;
        forever begin
            clock = ~clock;
            #50;
        end
    end

    initial begin
        reset_n = 1'd0;
        write_en = 1'd0;
        addr = 4'd0;
        #100;
        reset_n = 1'd1;
        wait (wait_request === 1'b0)
        #50;
        addr = 4'd1;
        write_en = 1'd1;
        data_in = 32'd0;
        #100;
        addr = 4'd2;
        data_in = 32'd0;
        #100;
        data_in = 32'd0;
        addr = 4'd3;
        #100;
        addr = 4'd4;
        #100;
        addr = 4'd5;
        data_in = 32'd1;
        #100;
        addr = 4'd6;
        // data_in = 32'd126976;
        data_in = 32'd0;
        #100;
        data_in = 32'd0;
        addr = 4'd7;
        #100;
        addr = 4'd8;
        #100;
        addr = 4'd9;
        data_in = 32'h1f;
        #100;
        addr = 4'd0;
        #100;
        write_en = 1'd0;
        #100;
        #100;
        wait (wait_request === 1'b0)
        read_en = 1'b1;
        #200;
        addr = 4'd1;
        #100;
        addr = 4'd2;
        #100;
        addr = 4'd3;
        #100;
        addr = 4'd4;
        #100;
        addr = 4'd5;
        #100;
        addr = 4'd0;
        
        #200;

        // ships = 5'b11111;
        // fired = {1'b1,{6{1'b0}},1'b1,{7{1'b0}},5'b11111,{4{1'b0}},5'b10101,{5{1'b0}},3'b111,{3{1'b0}},1'b1,{6{1'b0}},1'b1,{25{1'b0}}};
        // #100;
        // rst_n = 1'd1;
        // wait (DUT.done === 1'd1)
        // start <= 1'd1;
        // #200;
        // start <= 1'd0;
        // wait (DUT.done === 1'd1)
        // #100;
        $stop;
    end
endmodule: tb_ai_monitor