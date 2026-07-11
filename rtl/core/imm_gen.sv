module imm_gen(

    // 32-bit instruction from instruction memory
    input logic [31:0] instruction,
    
    //immediate value extracted from instruction
    output logic [31:0] imm
);

logic unused;

logic [6:0] opcode;

always_comb begin

    unused = &instruction[19:12];
    
    //Extract opcode field
    opcode = instruction[6:0];

    //Default value
    imm = 32'd0;

    case(opcode)

    // I-type immediates:
    // ADDI/SLTI, loads, and JALR
    7'b0010011,
    7'b0000011,
    7'b1100111: begin
        imm = {{20{instruction[31]}}, instruction[31:20]};
    end

    //S-Type Instructions
    //Used by store instructions (currently sw)
    7'b0100011:
    begin
        imm = {{20{instruction[31]}},
                instruction[31:25],
                instruction[11:7]};
    end

    //B-Type Instructions
    //Used by beq
    //Branch immediates are stored in a non-contiguous format
    7'b1100011:
    begin
        imm = {{19{instruction[31]}},
        instruction[31],
        instruction[7],
        instruction[30:25],
        instruction[11:8],
        1'b0};
    end

    7'b1101111: begin
    // J-type immediate used by JAL.
    //
    // Immediate layout:
    // imm[20]    = instruction[31]
    // imm[10:1]  = instruction[30:21]
    // imm[11]    = instruction[20]
    // imm[19:12] = instruction[19:12]
    // imm[0]     = 0
        imm = {
            {11{instruction[31]}},
            instruction[31],
            instruction[19:12],
            instruction[20],
            instruction[30:21],
            1'b0
        };
    end

    //For unsupported instruction type
    //Output the immediate defaults to zero
    default:

        imm = 32'd0;

    endcase

end

endmodule
