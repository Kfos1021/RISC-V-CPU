#include <iostream>
#include <cstdint>
#include "Vimem.h"

using namespace std;

// Executes one instrsuction memory test
bool check(Vimem* dut, uint32_t pc, uint32_t expected_instruction){
    // Apply PC to the instruction memory
    dut->pc = pc;

    // Evaluate combinational logic
    dut->eval();

    // Verify instruction output
    if(dut->instruction != expected_instruction){
        cout << "Fail\n";
        cout << "PC = " << pc << endl;
        cout << "Expected: 0x" << hex << expected_instruction << endl;
        cout << "Got: 0x" << dut->instruction << dec << endl;
        return false;
    }
    return true;
}

int main(){
    // Create instruction memory module
    Vimem* dut = new Vimem;
    bool pass = true;

    // PC = 0 -> first instruction
    pass &= check(dut, 0, 0x00A30293);
    
    // PC = 4 -> second instruction
    pass &= check(dut, 4, 0x007302B3);

    // PC = 8 -> third instruction
    pass &= check(dut, 8, 0x407302B3);

    // PC = 12 -> fourth instruction
    pass &= check(dut, 12, 0x00000013);

    if (pass)
        cout << "All instruction memory tests passed" << endl;
    else
        cout << "Instruction memory tests failed" << endl;

    delete dut;
    return pass ? 0 : 1;
}