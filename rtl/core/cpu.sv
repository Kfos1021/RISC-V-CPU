// Single-Cycle RISC-V CPU Top Level
// Connects together all datapath and control modules
// -program counter, instruction memory, decoder, control unit,
// immediate generator, register file, ALU, data memory
module cpu(
    input logic clk,
    input logic reset,

    output logic [31:0] pc_debug,
    output logic [31:0] instruction_debug,
    output logic [31:0] alu_result_debug
);

    //Internal signals
    // Connect inputs of one module to inputs of another throughout processor
    logic branch_taken;
    logic [31:0] branch_target;
    logic [31:0] pc_out;

    logic [31:0] instruction;

    logic [6:0] opcode;
    logic [4:0] rd;
    logic [2:0] funct3;
    logic [4:0] rs1;
    logic [4:0] rs2;
    logic [6:0] funct7;

    logic reg_write;
    logic alu_src;
    logic mem_read;
    logic mem_write;
    logic mem_to_reg;
    logic branch;
    logic [3:0] alu_op;

    logic [31:0] imm;

    logic [31:0] read_data1;
    logic [31:0] read_data2;

    logic [31:0] alu_b;
    logic [31:0] alu_result;

    logic [31:0] mem_read_data;
    logic [31:0] writeback_data;

    // Program counter, holds address of current instruction
    // and updates every clock cycle
    pc pc_inst(
        .clk(clk),
        .reset(reset),
        .branch_taken(branch_taken),
        .branch_target(branch_target),
        .pc_out(pc_out)
    );

    // Instruction memory, fetches the instruction
    // located at the current PC
    imem imem_inst(
        .pc(pc_out),
        .instruction(instruction)
    );

    // Instruction decoder
    // Splits the 32-bit instruction into opcode, register fields,
    // funct3, and funct7
    decoder decoder_inst(
        .instruction(instruction),
        .opcode(opcode),
        .rd(rd),
        .funct3(funct3),
        .rs1(rs1),
        .rs2(rs2),
        .funct7(funct7)
    );

    // Main control unit
    // Generates datapath control signals based on opcode,
    // funct3, and funct7
    control control_inst(
        .opcode(opcode),
        .funct3(funct3),
        .funct7(funct7),
        .reg_write(reg_write),
        .alu_src(alu_src),
        .mem_read(mem_read),
        .mem_write(mem_write),
        .mem_to_reg(mem_to_reg),
        .branch(branch),
        .alu_op(alu_op)
    );

    // Immediate generator
    // Extracts and sign-extends immediates from I-type,
    // S-type, and B-type instructions
    imm_gen imm_gen_inst(
        .instruction(instruction),
        .imm(imm)
    );

    // Register file, reads two source registers and optionally writes
    // one destination register each clock cycle
    regfile regfile_inst(
        .clk(clk),
        .we(reg_write),
        .rs1(rs1),
        .rs2(rs2),
        .rd(rd),
        .write_data(writeback_data),
        .read_data1(read_data1),
        .read_data2(read_data2)
    );

    assign alu_b = alu_src ? imm : read_data2;

    // ALU operand selection, choose between register operand and immediate value
    // depending on the instruction type
    alu alu_inst(
        .a(read_data1),
        .b(alu_b),
        .op(alu_op),
        .result(alu_result)
    );

    // Data memory, stores and loads data for memory instructions
    dmem dmem_inst(
        .clk(clk),
        .mem_read(mem_read),
        .mem_write(mem_write),
        .address(alu_result),
        .write_data(read_data2),
        .read_data(mem_read_data)
    );

    // Select whether the register file receives the ALU
    // result or data loaded from memory
    assign writeback_data = mem_to_reg ? mem_read_data : alu_result;

    // Branch decision logic
    assign branch_taken = branch && (read_data1 == read_data2);
    assign branch_target = pc_out + imm;

    assign pc_debug = pc_out;
    assign instruction_debug = instruction;
    assign alu_result_debug = alu_result;

endmodule
