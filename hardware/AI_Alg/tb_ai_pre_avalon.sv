module tb_ai();

    reg clk, rst_n, start;
    reg [4:0] ships;
    reg [99:0] fired;
    reg [99:0] hits;
    wire done;
    wire [6:0] largest_index;

    ai DUT(fired, hits, ships, clk, rst_n, largest_index, done, start);

    initial begin
        clk = 1'b1;
        forever begin
            clk = ~clk;
            #50;
        end
    end

    initial begin
        rst_n = 1'd0;
        ships = 5'b10101;
        fired = {1'b1,{6{1'b0}},1'b1,{7{1'b0}},5'b11111,{4{1'b0}},5'b10101,{5{1'b0}},3'b111,{3{1'b0}},1'b1,{6{1'b0}},1'b1,{25{1'b0}}};
        hits = 100'd0;
        hits = hits + {1'b1,{41{1'b0}}};
        #100;
        rst_n = 1'd1;
        wait (DUT.done === 1'd1)
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        #300;

        // Play through a game
        rst_n = 1'd0;
        ships = 5'b11111;
        fired = 100'd0;
        hits = 100'd0;
        #100;
        rst_n = 1'd1;
        wait (DUT.done === 1'd1)
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // First shot, miss at 44
        assert(largest_index === 7'd44); 
        fired = fired + {1'd1,{44{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Second shot hit battleship at 55
        assert(largest_index === 7'd55);
        hits = hits + {1'd1,{55{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 45
        assert(largest_index === 7'd45);
        fired = fired + {1'd1,{45{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit battleship at 54
        assert(largest_index === 7'd54);
        hits = hits + {1'd1,{54{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit battleship at 53
        assert(largest_index === 7'd53);
        hits = hits + {1'd1,{53{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit cruiser at 43
        assert(largest_index === 7'd43);
        hits = hits + {1'd1,{43{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 33
        assert(largest_index === 7'd33);
        fired = fired + {1'd1,{33{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit sunk battleship at 56
        assert(largest_index === 7'd56);
        fired = fired + {1'd1,{56{1'd0}}};
        fired = fired + {1'd1,{55{1'd0}}};
        fired = fired + {1'd1,{54{1'd0}}};
        fired = fired + {1'd1,{53{1'd0}}};
        hits = hits - {1'd1,{55{1'd0}}};
        hits = hits - {1'd1,{54{1'd0}}};
        hits = hits - {1'd1,{53{1'd0}}};
        ships = 5'b10111;
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit cruiser at 42
        assert(largest_index === 7'd42);
        hits = hits + {1'd1,{42{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 32
        assert(largest_index === 7'd32);
        fired = fired + {1'd1,{32{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit sunk cruiser at 41. Back to hunting
        assert(largest_index === 7'd41);
        fired = fired + {1'd1,{43{1'd0}}};
        fired = fired + {1'd1,{42{1'd0}}};
        fired = fired + {1'd1,{41{1'd0}}};
        hits = hits - {1'd1,{43{1'd0}}};
        hits = hits - {1'd1,{42{1'd0}}};
        ships = 5'b10011;
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 67
        assert(largest_index === 7'd67);
        fired = fired + {1'd1,{67{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 26
        assert(largest_index === 7'd26);
        fired = fired + {1'd1,{26{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit submarine at 38
        assert(largest_index === 7'd38);
        hits = hits + {1'd1,{38{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 48
        assert(largest_index === 7'd48);
        fired = fired + {1'd1,{48{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit submarine at 37
        assert(largest_index === 7'd37);
        hits = hits + {1'd1,{37{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 27
        assert(largest_index === 7'd27);
        fired = fired + {1'd1,{27{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit sunk submarine at 36. Back to hunting
        assert(largest_index === 7'd36);
        fired = fired + {1'd1,{38{1'd0}}};
        fired = fired + {1'd1,{37{1'd0}}};
        fired = fired + {1'd1,{36{1'd0}}};
        hits = hits - {1'd1,{38{1'd0}}};
        hits = hits - {1'd1,{37{1'd0}}};
        ships = 5'b10001;
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 14
        assert(largest_index === 7'd14);
        fired = fired + {1'd1,{14{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 74
        assert(largest_index === 7'd74);
        fired = fired + {1'd1,{74{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 85
        assert(largest_index === 7'd85);
        fired = fired + {1'd1,{85{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 5
        assert(largest_index === 7'd5);
        fired = fired + {1'd1,{5{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 50
        assert(largest_index === 7'd50);
        fired = fired + {1'd1,{50{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 59
        assert(largest_index === 7'd59);
        fired = fired + {1'd1,{59{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 62
        assert(largest_index === 7'd62);
        fired = fired + {1'd1,{62{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 94
        assert(largest_index === 7'd94);
        fired = fired + {1'd1,{94{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 21
        assert(largest_index === 7'd21);
        fired = fired + {1'd1,{21{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 78
        assert(largest_index === 7'd78);
        fired = fired + {1'd1,{78{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 81
        assert(largest_index === 7'd81);
        fired = fired + {1'd1,{81{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 18
        assert(largest_index === 7'd18);
        fired = fired + {1'd1,{18{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 0
        assert(largest_index === 7'd0);
        fired = fired + 1'd1;
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 12
        assert(largest_index === 7'd12);
        fired = fired + {1'd1,{12{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit carrier at 29
        assert(largest_index === 7'd29);
        hits = hits + {1'd1,{29{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit carrier at 19
        assert(largest_index === 7'd19);
        hits = hits + {1'd1,{19{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit carrier at 9
        assert(largest_index === 7'd9);
        hits = hits + {1'd1,{9{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit carrier at 39
        assert(largest_index === 7'd39);
        hits = hits + {1'd1,{39{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit sunk carrier at 49
        assert(largest_index === 7'd49);
        fired = fired + {1'd1,{49{1'd0}}};
        fired = fired + {1'd1,{39{1'd0}}};
        fired = fired + {1'd1,{28{1'd0}}};
        fired = fired + {1'd1,{19{1'd0}}};
        fired = fired + {1'd1,{9{1'd0}}};
        hits = hits - {1'd1,{39{1'd0}}};
        hits = hits - {1'd1,{29{1'd0}}};
        hits = hits - {1'd1,{19{1'd0}}};
        hits = hits - {1'd1,{9{1'd0}}};
        ships = 5'b00001;
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 76
        assert(largest_index === 7'd76);
        fired = fired + {1'd1,{76{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 83
        assert(largest_index === 7'd83);
        fired = fired + {1'd1,{83{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 87
        assert(largest_index === 7'd87);
        fired = fired + {1'd1,{87{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 3
        assert(largest_index === 7'd3);
        fired = fired + {1'd1,{3{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 7
        assert(largest_index === 7'd7);
        fired = fired + {1'd1,{7{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 16
        assert(largest_index === 7'd16);
        fired = fired + {1'd1,{16{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 23
        assert(largest_index === 7'd23);
        fired = fired + {1'd1,{23{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 25
        assert(largest_index === 7'd25);
        fired = fired + {1'd1,{25{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 30
        assert(largest_index === 7'd30);
        fired = fired + {1'd1,{30{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 61
        assert(largest_index === 7'd61);
        fired = fired + {1'd1,{61{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 65
        assert(largest_index === 7'd65);
        fired = fired + {1'd1,{65{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 70
        assert(largest_index === 7'd70);
        fired = fired + {1'd1,{70{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 72
        assert(largest_index === 7'd72);
        fired = fired + {1'd1,{72{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 89
        assert(largest_index === 7'd89);
        fired = fired + {1'd1,{89{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 92
        assert(largest_index === 7'd92);
        fired = fired + {1'd1,{92{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit on destroyer 96
        assert(largest_index === 7'd96);
        hits = hits + {1'd1,{96{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Miss at 86
        assert(largest_index === 7'd86);
        fired = fired + {1'd1,{86{1'd0}}};
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Hit at 95. Game over
        assert(largest_index === 7'd95);
        fired = fired + {1'd1,{96{1'd0}}};
        fired = fired + {1'd1,{95{1'd0}}};
        hits = hits - {1'd1,{96{1'd0}}};
        ships = 5'd0;
        start <= 1'd1;
        #200;
        start <= 1'd0;
        wait (DUT.done === 1'd1)
        // Largest is 0 becuase all desnity is 0. No ships
        assert(largest_index === 7'd0);
        #300;
        $stop;
    end
endmodule: tb_ai