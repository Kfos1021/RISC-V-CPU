module regfile(
    input logic clk,
    input logic we,
    
    // Two read registers
    input logic [4:0] rs1,
    input logic [4:0] rs2,
    
    // Destination register
    input logic [4:0] rd,

    input logic [31:0] write_data,
    output logic [31:0] read_data1,
    output logic [31:0] read_data2
);

    logic [31:0] regs [31:0];

    // Reading logic
    assign read_data1 = regs[rs1];
    assign read_data2 = regs[rs2];

    // Logic for writing to registers
    always_ff @(posedge clk) begin
        if(we && rd != 5'd0)
            regs[rd] <= write_data;

        regs[0] <= 32'd0;
    end

endmodule
