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

    // PC 0: addi x1, x0, 5
    pass &= check(
        dut,
        0,
        0x00500093,
        5,
        1,
        5,
        true
    );
    tick(dut);

    // PC 4: addi x2, x0, 3
    pass &= check(
        dut,
        4,
        0x00300113,
        3,
        2,
        3,
        true
    );
    tick(dut);

    // PC 8: add x3, x1, x2
    pass &= check(
        dut,
        8,
        0x002081B3,
        8,
        3,
        8,
        true
    );
    tick(dut);

    // PC 12: sub x4, x1, x2
    pass &= check(
        dut,
        12,
        0x40208233,
        2,
        4,
        2,
        true
    );
    tick(dut);

    // PC 16: and x5, x1, x2
    pass &= check(
        dut,
        16,
        0x0020F2B3,
        1,
        5,
        1,
        true
    );
    tick(dut);

    // PC 20: or x6, x1, x2
    pass &= check(
        dut,
        20,
        0x0020E333,
        7,
        6,
        7,
        true
    );
    tick(dut);

    // PC 24: xor x7, x1, x2
    pass &= check(
        dut,
        24,
        0x0020C3B3,
        6,
        7,
        6,
        true
    );
    tick(dut);

    // PC 28: sll x8, x2, x1
    // 3 << 5 = 96
    pass &= check(
        dut,
        28,
        0x00111433,
        96,
        8,
        96,
        true
    );
    tick(dut);

    // PC 32: srl x9, x8, x2
    // 96 >> 3 = 12
    pass &= check(
        dut,
        32,
        0x002454B3,
        12,
        9,
        12,
        true
    );
    tick(dut);

    // PC 36: sra x10, x9, x2
    // 12 >>> 3 = 1
    pass &= check(
        dut,
        36,
        0x4024D533,
        1,
        10,
        1,
        true
    );
    tick(dut);

    // PC 40: slt x11, x2, x1
    // Signed 3 < 5
    pass &= check(
        dut,
        40,
        0x001125B3,
        1,
        11,
        1,
        true
    );
    tick(dut);

    // PC 44: sltu x12, x1, x2
    // Unsigned 5 < 3 is false
    pass &= check(
        dut,
        44,
        0x0020B633,
        0,
        12,
        0,
        true
    );
    tick(dut);

        // PC 48: sw x3, 0(x0)
    // Address calculation result is 0.
    // No register write occurs.
    pass &= check(
        dut,
        48,
        0x00302023,
        0,
        0,
        0,
        false
    );
    tick(dut);

    // PC 52: lw x13, 0(x0)
    // Memory should contain the value 8.
    pass &= check(
        dut,
        52,
        0x00002683,
        0,
        13,
        8,
        true
    );
    tick(dut);

    // PC 56: beq x13, x3, 8
    // x13 == x3, so jump directly to PC 64.
    pass &= check(
        dut,
        56,
        0x00368463,
        0,
        8,
        0,
        false
    );
    tick(dut);

    // PC 60 must be skipped.

    // PC 64: lui x15, 0x12345
    pass &= check(
        dut,
        64,
        0x123457B7,
        0,              // ignored
        15,
        0x12345000,
        true,
        false
    );
    tick(dut);

    // PC 68: auipc x16, 0x1
    // 0x1000 + PC 68 = 0x1044
    pass &= check(
        dut,
        68,
        0x00001817,
        0,              // ignored
        16,
        0x00001044,
        true,
        false
    );
    tick(dut);

    // PC 72: jal x17, 8
    // x17 receives PC + 4 = 76.
    // Next PC becomes 80.
    pass &= check(
        dut,
        72,
        0x008008EF,
        0,              // ignored
        17,
        76,
        true,
        false
    );
    tick(dut);

    // PC 76 must be skipped.

    // PC 80: addi x18, x0, 88
    pass &= check(
        dut,
        80,
        0x05800913,
        88,
        18,
        88,
        true
    );
    tick(dut);

    // PC 84: addi x19, x0, 96
    pass &= check(
        dut,
        84,
        0x06000993,
        96,
        19,
        96,
        true
    );
    tick(dut);

    // PC 88: jalr x20, 0(x19)
    // x20 receives 92, and execution jumps to PC 96.
    pass &= check(
        dut,
        88,
        0x00098A67,
        96,
        20,
        92,
        true
    );
    tick(dut);

    // PC 92 must be skipped.

    // PC 96: addi x21, x0, 77
    pass &= check(
        dut,
        96,
        0x04D00A93,
        77,
        21,
        77,
        true
    );
    tick(dut);

    // PC 100: nop
    pass &= check(
        dut,
        100,
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