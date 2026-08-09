#include <iostream>
#include <cstdint>

#include "Vcpu.h"
#include "verilated.h"

using namespace std;

void tick(Vcpu* dut) {
    dut->clk = 0;
    dut->eval();

    dut->clk = 1;
    dut->eval();

    dut->clk = 0;
    dut->eval();
}

int main() {
    Vcpu* dut = new Vcpu;

    dut->clk = 0;
    dut->reset = 1;

    tick(dut);

    dut->reset = 0;

    // Run the compiled C program long enough to reach its infinite loop.
    for (int cycle = 0; cycle < 100; cycle++) {
        tick(dut);
    }

    bool pass = true;

    if (dut->signature_debug != 8) {
        cout << "Fail: expected result 8, got "
             << dut->signature_debug << endl;
        pass = false;
    }

    if (pass) {
        cout << "Compiled C program passed" << endl;
    } else {
        cout << "Compiled C program failed" << endl;
    }

    delete dut;
    return pass ? 0 : 1;
}