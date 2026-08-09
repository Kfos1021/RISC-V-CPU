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

        // PC 0: lui x1, 0x80ff8
    pass &= check(
        dut,
        0,
        0x80FF80B7,
        0,                  // ALU output is not relevant for LUI
        1,
        0x80FF8000,
        true,
        false
    );
    tick(dut);

    // PC 4: addi x1, x1, -255
    // 0x80FF8000 - 255 = 0x80FF7F01
    pass &= check(
        dut,
        4,
        0xF0108093,
        0x80FF7F01,
        1,
        0x80FF7F01,
        true
    );
    tick(dut);

    // PC 8: sw x1, 0(x0)
    pass &= check(
        dut,
        8,
        0x00102023,
        0,
        0,
        0,
        false
    );
    tick(dut);

    // PC 12: lb x2, 0(x0)
    pass &= check(
        dut,
        12,
        0x00000103,
        0,
        2,
        0x00000001,
        true
    );
    tick(dut);

    // PC 16: lbu x3, 2(x0)
    pass &= check(
        dut,
        16,
        0x00204183,
        2,
        3,
        0x000000FF,
        true
    );
    tick(dut);

    // PC 20: lb x4, 2(x0)
    pass &= check(
        dut,
        20,
        0x00200203,
        2,
        4,
        0xFFFFFFFF,
        true
    );
    tick(dut);

    // PC 24: lhu x5, 2(x0)
    pass &= check(
        dut,
        24,
        0x00205283,
        2,
        5,
        0x000080FF,
        true
    );
    tick(dut);

    // PC 28: lh x6, 2(x0)
    pass &= check(
        dut,
        28,
        0x00201303,
        2,
        6,
        0xFFFF80FF,
        true
    );
    tick(dut);

    // PC 32: addi x7, x0, 0x55
    pass &= check(
        dut,
        32,
        0x05500393,
        0x55,
        7,
        0x55,
        true
    );
    tick(dut);

    // PC 36: sb x7, 1(x0)
    // Memory changes from 0x80FF7F01 to 0x80FF5501.
    pass &= check(
        dut,
        36,
        0x007000A3,
        1,
        1,                  // Store immediate occupies the rd bit positions
        1,
        false
    );
    tick(dut);

    // PC 40: lw x8, 0(x0)
    pass &= check(
        dut,
        40,
        0x00002403,
        0,
        8,
        0x80FF5501,
        true
    );
    tick(dut);

    // PC 44: lui x9, 0xB
    pass &= check(
        dut,
        44,
        0x0000B4B7,
        0,
        9,
        0x0000B000,
        true,
        false
    );
    tick(dut);

    // PC 48: addi x9, x9, -0x545
    // 0xB000 - 0x545 = 0xAABB
    pass &= check(
        dut,
        48,
        0xABB48493,
        0x0000AABB,
        9,
        0x0000AABB,
        true
    );
    tick(dut);

    // PC 52: sh x9, 2(x0)
    // Memory changes from 0x80FF5501 to 0xAABB5501.
    pass &= check(
        dut,
        52,
        0x00901123,
        2,
        2,                  // Store immediate occupies the rd bit positions
        2,
        false
    );
    tick(dut);

    // PC 56: lw x10, 0(x0)
    pass &= check(
        dut,
        56,
        0x00002503,
        0,
        10,
        0xAABB5501,
        true
    );
    tick(dut);

    // PC 60: nop
    pass &= check(
        dut,
        60,
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