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

    // PC 0: invalid instruction
    dut->eval();

    if (dut->pc_debug != 0) {
        cout << "Fail: expected PC 0, got "
            << dut->pc_debug << endl;
        pass = false;
    }

    if (dut->instruction_debug != 0xFFFFFFFF) {
        cout << "Fail: expected illegal instruction 0xFFFFFFFF, got 0x"
            << hex << dut->instruction_debug << dec << endl;
        pass = false;
    }

    if (dut->illegal_instruction_debug != 1) {
        cout << "Fail: illegal instruction was not detected"
            << endl;
        pass = false;
    }

    if (dut->reg_write_debug != 0) {
        cout << "Fail: illegal instruction enabled register write"
            << endl;
        pass = false;
    }

    // First clock cycle after detection.
    // The CPU should remain at PC 0.
    tick(dut);

    if (dut->pc_debug != 0) {
        cout << "Fail: CPU did not halt at PC 0"
            << endl;
        pass = false;
    }

    if (dut->instruction_debug != 0xFFFFFFFF) {
        cout << "Fail: halted CPU changed instruction"
            << endl;
        pass = false;
    }

    if (dut->illegal_instruction_debug != 1) {
        cout << "Fail: illegal signal cleared while halted"
            << endl;
        pass = false;
    }

    if (dut->reg_write_debug != 0) {
        cout << "Fail: halted CPU enabled register write"
            << endl;
        pass = false;
    }

    // Tick again to prove the halt is persistent.
    tick(dut);

    if (dut->pc_debug != 0) {
        cout << "Fail: halted CPU advanced after another cycle"
            << endl;
        pass = false;
    }

    if (dut->illegal_instruction_debug != 1) {
        cout << "Fail: illegal signal did not remain asserted"
            << endl;
        pass = false;
    }

    if (pass) {
    cout << "Illegal instruction halt test passed" << endl;
    } else {
    cout << "Illegal instruction halt test failed" << endl;
    }

    delete dut;

    // Return success if every test passed
    return pass ? 0 : 1;

}