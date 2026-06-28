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
        memory[0] = 32'h00500093; // addi x1, x0, 5
        memory[1] = 32'h00A00113; // addi x2, x0, 10
        memory[2] = 32'h002081B3; // add x3, x1, x2
        memory[3] = 32'h40110233; // sub x4, x2, x1
        memory[4] = 32'h00000013; // nop
    end

    //Convert byte address into a word index
    // Bits [1:0] are always zero because instructions are 4-byte aligned.
    // Bits [9:2] allow addressing all 256 instruction words.
    assign instruction = memory[pc[9:2]];

endmodule

/* verilator lint_on UNUSEDSIGNAL */
