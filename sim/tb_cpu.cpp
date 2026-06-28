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
    uint32_t expected_alu_result
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

    // Cycle 0: addi x5, x6, 10
    pass &= check(dut, 0, 0x00A30293, 10);
    tick(dut);

    // Cycle 1: add x5, x6, x7
    pass &= check(dut, 4, 0x007302B3, 0);
    tick(dut);

    // Cycle 2: sub x5, x6, x7
    pass &= check(dut, 8, 0x407302B3, 0);
    tick(dut);

    // Cycle 3: nop
    pass &= check(dut, 12, 0x00000013, 0);
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