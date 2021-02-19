module tb_ai();

    reg clk, rst_n;
    reg [4:0] ships;
    reg [99:0] fired;
    wire [99:0][5:0] density;

    ai DUT(fired, ships, clk, rst_n, density);

    initial begin
        clk = 1'b1;
        forever begin
            clk = ~clk;
            #50;
        end
    end

    initial begin
        rst_n = 1'd0;
        ships = 5'b11111;
        fired = 100'd0;
        #100;
        rst_n = 1'd1;
        #33000;
        $stop;
    end
endmodule: tb_ai