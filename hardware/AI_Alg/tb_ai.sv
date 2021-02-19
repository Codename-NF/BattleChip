module tb_ai();

    reg clk, rst_n, start;
    reg [4:0] ships;
    reg [99:0] fired;
    wire done;
    wire [6:0] largest_index;
    wire [99:0][5:0] density;

    ai DUT(fired, ships, clk, rst_n, density, largest_index, done, start);

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
        fired = {1'b1,{6{1'b0}},1'b1,{7{1'b0}},5'b11111,{4{1'b0}},5'b10101,{5{1'b0}},3'b111,{3{1'b0}},1'b1,{6{1'b0}},1'b1,{25{1'b0}}};
        #100;
        rst_n = 1'd1;
        wait (DUT.done === 1'd1)
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        #100;
        $stop;
    end
endmodule: tb_ai