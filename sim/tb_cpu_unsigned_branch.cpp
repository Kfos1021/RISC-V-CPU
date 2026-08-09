#include <iostream>
#include <cstdint>

#include "Vcpu.h"
#include "verilated.h"

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
    bool expected_reg_write,
    bool check_alu = true
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

    if(check_alu && dut->alu_result_debug != expected_alu_result){
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

        // Cycle 0: addi x1, x0, -1
    pass &= check(
        dut,
        0,
        0xFFF00093,
        0xFFFFFFFF,
        1,
        0xFFFFFFFF,
        true
    );
    tick(dut);

    // Cycle 1: addi x2, x0, 1
    pass &= check(
        dut,
        4,
        0x00100113,
        1,
        2,
        1,
        true
    );
    tick(dut);

    // Cycle 2: bltu x1, x2, +8
    // Unsigned 0xFFFFFFFF < 1 is false, so the branch is not taken.
    pass &= check(
        dut,
        8,
        0x0020E463,
        0xFFFFFFFE,
        8,
        0xFFFFFFFE,
        false
    );
    tick(dut);

    // Cycle 3: addi x3, x0, 3
    pass &= check(
        dut,
        12,
        0x00300193,
        3,
        3,
        3,
        true
    );
    tick(dut);

    // Cycle 4: bgeu x1, x2, +8
    // Unsigned 0xFFFFFFFF >= 1 is true, so PC 20 is skipped.
    pass &= check(
        dut,
        16,
        0x0020F463,
        0xFFFFFFFE,
        8,
        0xFFFFFFFE,
        false
    );
    tick(dut);

    // Cycle 5: addi x4, x0, 4 at PC 24
    pass &= check(
        dut,
        24,
        0x00400213,
        4,
        4,
        4,
        true
    );
    tick(dut);

    // Cycle 6: bltu x2, x1, +8
    // Unsigned 1 < 0xFFFFFFFF is true, so PC 32 is skipped.
    pass &= check(
        dut,
        28,
        0x00116463,
        2,
        8,
        2,
        false
    );
    tick(dut);

    // Cycle 7: addi x5, x0, 5 at PC 36
    pass &= check(
        dut,
        36,
        0x00500293,
        5,
        5,
        5,
        true
    );
    tick(dut);

    // Cycle 8: bgeu x2, x1, +8
    // Unsigned 1 >= 0xFFFFFFFF is false, so the branch is not taken.
    pass &= check(
        dut,
        40,
        0x00117463,
        2,
        8,
        2,
        false
    );
    tick(dut);

    // Cycle 9: addi x6, x0, 99
    pass &= check(
        dut,
        44,
        0x06300313,
        99,
        6,
        99,
        true
    );
    tick(dut);

    // Cycle 10: addi x6, x0, 6
    pass &= check(
        dut,
        48,
        0x00600313,
        6,
        6,
        6,
        true
    );
    tick(dut);

    // Cycle 11: nop
    pass &= check(
        dut,
        52,
        0x00000013,
        0,
        0,
        0,
        true
    );
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