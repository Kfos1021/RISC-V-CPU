#include <iostream>
#include <cstdint>
#include "Vcpu.h"

using namespace std;

// Simulate one complete processor clock cycle
// Rising edge updates all sequential logic
void tick(Vcpu* dut){
    dut->clk = 0;
    dut->eval();

    dut->clk = 1;
    dut->eval();

    dut->clk = 0;
    dut->eval();
}

// Verify the CPU produced the expected outputs for current clock cycle
bool check(
    // Assume cycle passes unless a mismatch is found
    Vcpu* dut,
    uint32_t expected_pc,
    uint32_t expected_instruction,
    uint32_t expected_alu_result,
    uint8_t expected_rd,
    uint32_t expected_writeback,
    bool expected_reg_write
) {
    bool pass = true;

    if(dut->pc_debug != expected_pc){
        cout << "Fail PC: expected " << expected_pc
             << " got " << dut->pc_debug << endl;
        pass = false;
    }

    if(dut->instruction_debug != expected_instruction){
        cout << "Fail instruction: expected 0x" << hex << expected_instruction
             << " got 0x" << dut->instruction_debug << dec << endl;
        pass = false;
    }

    if(dut->alu_result_debug != expected_alu_result){
        cout << "Fail ALU result: expected " << expected_alu_result
             << " got " << dut->alu_result_debug << endl;
        pass = false;
    }

    if(dut->rd_debug != expected_rd){
        cout << "Fail rd: expected x" << unsigned(expected_rd)
             << " got x" << unsigned(dut->rd_debug) << endl;
        pass = false;
    }

    if(dut->writeback_debug != expected_writeback){
        cout << "Fail writeback: expected " << expected_writeback
             << " got " << dut->writeback_debug << endl;
        pass = false;
    }

    if(dut->reg_write_debug != expected_reg_write){
        cout << "Fail reg_write: expected " << expected_reg_write
             << " got " << unsigned(dut->reg_write_debug) << endl;
        pass = false;
    }

    return pass;
}

int main(){
    // Create CPU model
    Vcpu* dut = new Vcpu;

    dut->clk = 0;
    dut->reset = 1;

    tick(dut);

    dut->reset = 0;

    bool pass = true;

    // Cycle 0: addi x1, x0, 5
    pass &= check(dut, 0, 0x00500093, 5, 1, 5, true);
    tick(dut);

    // Cycle 1: addi x2, x0, 5
    pass &= check(dut, 4, 0x00500113, 5, 2, 5, true);
    tick(dut);

    // Cycle 2: beq x1, x2, +8
    // Equal, so jump from PC 8 to PC 16.
    pass &= check(dut, 8, 0x00208463, 0, 8, 0, false);
    tick(dut);

    // Cycle 3: PC 12 was skipped.
    // addi x3, x0, 1
    pass &= check(dut, 16, 0x00100193, 1, 3, 1, true);
    tick(dut);

    // Cycle 4: bne x1, x2, +8
    // Equal, so BNE is not taken. Next PC is 24.
    pass &= check(dut, 20, 0x00209463, 0, 8, 0, false);
    tick(dut);

    // Cycle 5: addi x4, x0, 2
    pass &= check(dut, 24, 0x00200213, 2, 4, 2, true);
    tick(dut);

    // Cycle 6: addi x2, x0, 7
    pass &= check(dut, 28, 0x00700113, 7, 2, 7, true);
    tick(dut);

    // Cycle 7: blt x1, x2, +8
    // Signed 5 < 7, so jump from PC 32 to PC 40.
    // ALU performs 5 - 7, producing unsigned 0xFFFFFFFE.
    pass &= check(
        dut,
        32,
        0x0020C463,
        0xFFFFFFFE,
        8,
        0xFFFFFFFE,
        false
    );
    tick(dut);

    // Cycle 8: PC 36 was skipped.
    // addi x5, x0, 3
    pass &= check(dut, 40, 0x00300293, 3, 5, 3, true);
    tick(dut);

    // Cycle 9: bge x2, x1, +8
    // Signed 7 >= 5, so jump from PC 44 to PC 52.
    pass &= check(dut, 44, 0x00115463, 2, 8, 2, false);
    tick(dut);

    // Cycle 10: PC 48 was skipped.
    // addi x6, x0, 4
    pass &= check(dut, 52, 0x00400313, 4, 6, 4, true);
    tick(dut);

    // Cycle 11: nop
    pass &= check(dut, 56, 0x00000013, 0, 0, 0, true);
    tick(dut);

    if (pass){
        cout << "All CPU integration tests passed" << endl;
    }
    else{
        cout << "CPU integration tests failed" << endl;
    }

    delete dut;
    // Return success if every test passed
    return pass ? 0 : 1;

}