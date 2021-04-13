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
        data_in = 32'd0;
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
        data_in = 32'd31;
        #100;
        addr = 4'd0;
        data_in = 32'd0;
        #100;
        write_en = 1'd0;
        wait (wait_request === 1'd0);
        #50;
        read_en = 1'd1;
        #100;
        $stop;
    end
endmodule: tb_ai