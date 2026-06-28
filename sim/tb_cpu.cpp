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

int main(){
    // Create CPU model
    Vcpu* dut = new Vcpu;

    dut->clk = 0;
    dut->reset = 1;

    tick(dut);

    dut->reset = 0;

    for(int i = 0; i < 5; i++){
        cout << "Cycle " << i << endl;
        cout << "PC   =  "  << dut->pc_debug << endl;
        cout << "Instruction = 0x" << hex << dut->instruction_debug << dec << endl;
        cout << "ALU Result = " << dut->alu_result_debug << endl;
        cout << "---------------------" << endl;

        tick(dut);
    }

    delete dut;
    return 0;
}