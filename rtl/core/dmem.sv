/* verilator lint_off UNUSEDSIGNAL */
module dmem(
    // Writes happen only on a clock edge
    input logic clk,

    // When 1, output memory value
    input logic mem_read,

    // When 1, store write_data
    input logic mem_write,

    input logic [2:0] funct3, 

    input logic [31:0] address,

    // Value from register file to store
    input logic [31:0] write_data,

    output logic [31:0] signature_debug,

    // Value loaded from memory 
    output logic [31:0] read_data

);
    
    logic [31:0] memory [0:255];
    // Expose data-memory word 0 as a program result/signature.
    assign signature_debug = memory[0];
    
    // Synchronous write logic
    // Stores data only on the rising edge of the clock
    always_ff @(posedge clk) begin
        if (mem_write) begin
            case (funct3)
                3'b000: begin
                    // SB
                    case (address[1:0])
                        2'b00:
                            memory[address[9:2]][7:0]
                                <= write_data[7:0];

                        2'b01:
                            memory[address[9:2]][15:8]
                                <= write_data[7:0];

                        2'b10:
                            memory[address[9:2]][23:16]
                                <= write_data[7:0];

                        2'b11:
                            memory[address[9:2]][31:24]
                                <= write_data[7:0];
                    endcase
                end

                3'b001: begin
                    // SH
                    case (address[1:0])
                        2'b00:
                            memory[address[9:2]][15:0]
                                <= write_data[15:0];

                        2'b10:
                            memory[address[9:2]][31:16]
                                <= write_data[15:0];

                        default: begin
                            // Misaligned halfword store: do nothing for now.
                        end
                    endcase
                end

                3'b010: begin
                    // SW
                    memory[address[9:2]] <= write_data;
                end

                default: begin
                    // Other store widths will be added next.
                end
            endcase
        end
    end

    // Combinational read logic
    // funct3 selects the load width.
    always_comb begin
        read_data = 32'd0;
        if (mem_read) begin
            case (funct3)
                3'b010: begin
                    // LW: return the complete 32-bit word.
                    read_data = memory[address[9:2]];
                end

                3'b100: begin
                    // LBU: select one byte and zero-extend it.
                    case (address[1:0])
                        2'b00:
                            read_data = {
                                24'd0,
                                memory[address[9:2]][7:0]
                            };

                        2'b01:
                            read_data = {
                                24'd0,
                                memory[address[9:2]][15:8]
                            };

                        2'b10:
                            read_data = {
                                24'd0,
                                memory[address[9:2]][23:16]
                            };

                        2'b11:
                            read_data = {
                                24'd0,
                                memory[address[9:2]][31:24]
                            };
                    endcase
                end

                3'b000: begin
                    // LB: select one byte and sign-extend it.
                    case (address[1:0])
                        2'b00:
                            read_data = {
                                {24{memory[address[9:2]][7]}},
                                memory[address[9:2]][7:0]
                            };

                        2'b01:
                            read_data = {
                                {24{memory[address[9:2]][15]}},
                                memory[address[9:2]][15:8]
                            };

                        2'b10:
                            read_data = {
                                {24{memory[address[9:2]][23]}},
                                memory[address[9:2]][23:16]
                            };

                        2'b11:
                            read_data = {
                                {24{memory[address[9:2]][31]}},
                                memory[address[9:2]][31:24]
                            };
                    endcase
                end

                3'b101: begin
                    // LHU: select one halfword and zero-extend it.
                    case (address[1])
                        1'b0:
                            read_data = {
                                16'd0,
                                memory[address[9:2]][15:0]
                            };

                        1'b1:
                            read_data = {
                                16'd0,
                                memory[address[9:2]][31:16]
                            };
                    endcase
                end

                3'b001: begin
                    // LH: select one halfword and sign-extend it.
                    case (address[1])
                        1'b0:
                            read_data = {
                                {16{memory[address[9:2]][15]}},
                                memory[address[9:2]][15:0]
                            };

                        1'b1:
                            read_data = {
                                {16{memory[address[9:2]][31]}},
                                memory[address[9:2]][31:16]
                            };
                    endcase
                end
                
                default: begin
                    read_data = 32'd0;
                end
            endcase
        end
    end

endmodule
/* verilator lint_on UNUSEDSIGNAL */
