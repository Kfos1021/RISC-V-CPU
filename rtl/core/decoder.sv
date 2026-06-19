module decoder(
    // Decoded instruction fields
    input logic [31:0] instruction,

    output logic [6:0] opcode,
    output logic [4:0] rd,
    output logic [2:0] funct3,
    output logic [4:0] rs1,
    output logic [4:0] rs2,
    output logic [6:0] funct7

);
//Extract opcode (bits 6-0)
//Tells CPU the class of the instruction
assign opcode = instruction[6:0];

//Extract destination register address (bits 11-7)
//Register where result will be written
assign rd = instruction[11:7];

//Extract function 3 field (bits 14-12)
//Specify operation
assign funct3 = instruction[14:12];

//Source registers
//Extract source register 1 address (bits 19-15)
assign rs1 = instruction[19:15];

//Extract source register 2 address (bits 24-20)
assign rs2 = instruction[24:20];

//Destinguish operations
//Extract funct7 field (bits 31-25)
assign funct7 = instruction[31:25];

endmodule
