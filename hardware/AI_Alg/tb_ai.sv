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
        rst_n = 1'd0;
        #100;
        rst_n = 1'd1;
        wait (wait_request === 1'd0)
        #100;
        $stop;
    end
endmodule: tb_ai