/* verilator lint_off UNUSEDSIGNAL */
module imem(
    // Current program counter
    input logic [31:0] pc,
    
    // Store instruction at requested address
    output logic [31:0] instruction
);
    // 256 word instruction memory
    logic [31:0] memory [0:255];

    // Initialize instruction memory with a small demo program
    initial begin
        memory[0] = 32'h00A30293; // addi x5, x6, 10
        memory[1] = 32'h007302B3; // add x5, x6, x7
        memory[2] = 32'h407302B3; // sub x5, x6, x7
        memory[3] = 32'h00000013; // nop
    end

    //Convert byte address into a word index
    // Bits [1:0] are always zero because instructions are 4-byte aligned.
    // Bits [9:2] allow addressing all 256 instruction words.
    assign instruction = memory[pc[9:2]];

endmodule

/* verilator lint_on UNUSEDSIGNAL */
