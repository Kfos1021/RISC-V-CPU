//hardware module "alu"
module alu(
    //32-bit operands
    input logic [31:0] a,
    input logic [31:0] b,
    //4-bit operation selector
    input logic [3:0] op,
    //32-bit output
    output logic [31:0] result
);

    always_comb begin
        //selects operation based on operation code
        case (op)
            //ex: op = 0 -> ADD
            4'd0: result = a + b;
            //SUBTRACT
            4'd1: result = a - b;
            //Bitwise AND
            4'd2: result = a & b;
            //Bitwise OR
            4'd3: result = a | b;
            //Bitwise XOR
            4'd4: result = a ^ b;
            //Set less than
            4'd5: result = ($signed(a) < $signed(b)) ? 32'd1 : 32'd0;
            //Shift left
            4'd6: result = a << b[4:0];
            //Shift right
            4'd7: result = a >> b[4:0];
            //SRA
            4'd8: result = $signed(a) >>> b[4:0];
            
            default: result = 32'd0;
        endcase
    end

endmodule
