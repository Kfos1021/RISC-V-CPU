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
        $readmemh("programs/cpu_test.hex", memory);
    end

    //Convert byte address into a word index
    // Bits [1:0] are always zero because instructions are 4-byte aligned.
    // Bits [9:2] allow addressing all 256 instruction words.
    assign instruction = memory[pc[9:2]];

endmodule

/* verilator lint_on UNUSEDSIGNAL */
