module pc(
    // System clock
    input logic clk,

    // Resets the PC back to address 0
    input logic reset,

    // Allows the PC to advance.
    // When low, the PC holds its current value.
    input logic enable,

    // If asserted, PC jumps to branch_target
    input logic branch_taken,
    input logic [31:0] branch_target,

    // Current program counter value
    output logic [31:0] pc_out
);

    always_ff @(posedge clk) begin
        if (reset) begin
            pc_out <= 32'd0;
        end
        else if (enable) begin
            if (branch_taken)
                pc_out <= branch_target;
            else
                pc_out <= pc_out + 32'd4;
        end
    end

endmodule
