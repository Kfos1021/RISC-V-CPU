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

    // Cycle 0: addi x1, x0, 15
    pass &= check(
        dut,
        0,
        0x00F00093,
        15,
        1,
        15,
        true
    );
    tick(dut);

    // Cycle 1: andi x2, x1, 10
    pass &= check(
        dut,
        4,
        0x00A0F113,
        10,
        2,
        10,
        true
    );
    tick(dut);

    // Cycle 2: ori x3, x1, 5
    pass &= check(
        dut,
        8,
        0x0050E193,
        15,
        3,
        15,
        true
    );
    tick(dut);

    // Cycle 3: xori x4, x1, 3
    pass &= check(
        dut,
        12,
        0x0030C213,
        12,
        4,
        12,
        true
    );
    tick(dut);

    // Cycle 4: nop
    pass &= check(
        dut,
        16,
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