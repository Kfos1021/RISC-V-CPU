/* verilator lint_off UNUSEDSIGNAL */
module dmem(
    // Writes happen only on a clock edge
    input logic clk,

    // When 1, output memory value
    input logic mem_read,

    // When 1, store write_data
    input logic mem_write,

    input logic [31:0] address,

    // Value from register file to store
    input logic [31:0] write_data,

    // Value loaded from memory 
    output logic [31:0] read_data
);
    
    logic [31:0] memory [0:255];
    
    // Synchronous write logic
    // Stores data only on the rising edge of the clock
    always_ff @(posedge clk) begin
        if(mem_write)
            memory[address[9:2]] <= write_data;
    end

    // Combinational read logic
    // Outputs memory data immediately when mem_read is enabled
    always_comb begin
        if(mem_read)
            read_data = memory[address[9:2]];
        else
            read_data = 32'd0;
    end

endmodule
/* verilator lint_on UNUSEDSIGNAL */
