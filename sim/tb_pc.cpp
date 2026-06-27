#include <iostream>
#include <cstdint>
#include "Vpc.h"

using namespace std;

// Simulates one complete clock cycle
// Rising edge updates the Program Counter
void tick(Vpc* dut){
    dut->clk = 0;
    dut->eval();

    dut->clk = 1;
    dut->eval();

    dut->clk = 0;
    dut->eval();
}

// Verify the Program Counter matches the expected value
bool check(Vpc* dut, uint32_t expected_pc){
    if(dut->pc_out != expected_pc){
        cout << "Fail: expected PC = " << expected_pc 
             << " got PC " << dut->pc_out << endl;  
        return false;
    }
    return true;
}


int main(){
    // Create Program Counter
    Vpc* dut = new Vpc;
    bool pass = true;

    dut->clk = 0;
    dut->reset = 1;
    dut->branch_taken = 0;
    dut->branch_target = 0;

    // Verify reset initializes the PC to address 0
    tick(dut);
    pass &= check(dut, 0);

    dut->reset = 0;

    // After reset, PC should increment by 4 bytes
    tick(dut);
    pass &= check(dut, 4);

    // Continue incrementing by 4
    tick(dut);
    pass &= check(dut, 8);

    // Simulate taking a branch
    dut->branch_taken = 1;
    dut->branch_target = 40;

    // Continue incrementing by 4
    tick(dut);
    pass &= check(dut, 40);

    dut->branch_taken = 0;

    tick(dut);
    pass &= check(dut, 44);

    // Print test results
    if(pass)
        cout << "All PC tests passed" << endl;
    else
        cout << "PC tests failed" << endl;

    delete dut;
    return pass ? 0 : 1;
}