module control(
    // Decode instruction fields from decoder
    input logic [6:0] opcode,
    input logic [2:0] funct3,
    input logic [6:0] funct7,

    // Control signals sent to datapath
    output logic reg_write,
    output logic alu_src,
    output logic mem_read,
    output logic mem_write,
    output logic mem_to_reg,
    output logic branch,
    output logic jump,

    //ALU operation selector
    //ADD -> 0, SUB -> 1, AND -> 2, OR -> 3, XOR -> 4, SLT -> 5, SLL -> 6, SRL -> 7
    output logic [3:0] alu_op
);

    //output updates immediately
    always_comb begin

        //Default values
        reg_write = 0;
        alu_src = 0;
        mem_read = 0;
        mem_write = 0;
        mem_to_reg = 0;
        branch= 0;
        jump = 0;
        alu_op = 0;

        case(opcode)

            // R-type ALU instructions: ADD, SUB, AND, OR, XOR, SLT, SLL, SRL
            7'b0110011: begin
                reg_write = 1;
                alu_src = 0;

                case(funct3)
                    3'b000: begin
                        if (funct7 == 7'b0100000)
                            alu_op = 4'd1; //SUB
                        else
                            alu_op = 4'd0; //ADD
                    end

                    3'b111: alu_op = 4'd2; // AND
                    3'b110: alu_op = 4'd3; // OR
                    3'b100: alu_op = 4'd4; // XOR
                    3'b010: alu_op = 4'd5; // SLT
                    3'b001: alu_op = 4'd6; // SLL

                    3'b101: begin
                        if(funct7 == 7'b0100000)
                            alu_op = 4'd8; // SRA
                        else
                            alu_op = 4'd7; // SRL
                    end

                    default: alu_op = 4'd0;
                endcase
            end

            //I-type arithmetic, currently ADDI
            7'b0010011: begin
                reg_write = 1;
                alu_src   = 1;

                case (funct3)
                    3'b000: alu_op = 4'd0; // ADDI
                    3'b010: alu_op = 4'd5; // SLTI
                    3'b100: alu_op = 4'd4; // XORI
                    3'b110: alu_op = 4'd3; // ORI
                    3'b111: alu_op = 4'd2; // ANDI
                    default: alu_op = 4'd0;
                endcase
            end

            // Jump and link: JAL
            7'b1101111: begin
                reg_write = 1;
                jump      = 1;

                // These make the ALU output predictable for testing,
                // although JAL does not use the ALU result for writeback.
                alu_src   = 1;
                alu_op    = 4'd0;
            end

            // Jump and link register: JALR
            7'b1100111: begin
                reg_write = 1;
                alu_src   = 1;
                jump      = 1;
                alu_op    = 4'd0; // ADD rs1 + immediate
            end

            //Load word: lw
            7'b0000011: begin
                reg_write = 1;
                alu_src = 1;
                mem_read = 1;
                mem_to_reg = 1;
                alu_op = 4'd0;
            end

            // Conditional branch instructions:
            // BEQ, BNE, BLT, and BGE
            7'b1100011: begin
                branch = 1;
                reg_write = 0;
                alu_src   = 0;
                alu_op = 4'd1;
            end

            //Store word: sw
             7'b0100011: begin
                alu_src = 1;
                mem_write = 1;
                alu_op = 4'd0;
            end


            //Unknown instructions, do nothing
            default: begin
                reg_write = 0;
                alu_src = 0;
                mem_read = 0;
                mem_write = 0;
                mem_to_reg = 0;
                branch = 0;
                alu_op = 0;
            end
        endcase
    end
endmodule
