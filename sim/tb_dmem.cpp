#include <iostream>
#include <cstdint>
#include "Vdmem.h"

using namespace std;

// Simulate one complete clock cycle
// Memory writes occur on the rising edge
void tick(Vdmem* dut){
    dut->clk = 0;
    dut->eval();

    dut->clk = 1;
    dut->eval();

    dut->clk = 0;
    dut->eval();
}

// Write a 32-bit value into data memory
void write_mem(Vdmem* dut, uint32_t address, uint32_t value){
    // Enable write operation and disable reads
    dut->mem_write = 1;
    dut->mem_read = 0;

    dut->address = address;
    dut->write_data = value;

    tick(dut);

    // Disable writes after completion
    dut->mem_write = 0;
    dut->eval();
}

// Read a value from data memory and verify
// that it matches the expected result.
bool check_read(Vdmem* dut, uint32_t address, uint32_t expected){
    // Enable read operation
    dut->mem_write = 0;
    dut->mem_read = 1;

    dut->address = address;
    dut->eval();

    if(dut->read_data != expected){
        cout << "Fail: address " << address
             << " expected " << expected
             << " got " << dut->read_data << endl;
        return false;
    }

    dut->mem_read = 0;
    return true;
}

int main(){

    // Create the Verilated data memory model
    Vdmem* dut = new Vdmem;
    bool pass = true;

    // Initialize all inputs to known values
    dut->clk = 0;
    dut->mem_read = 0;
    dut->mem_write = 0;
    dut->address = 0;
    dut->write_data = 0;
    dut->eval();

    // Write 123 to address 0 and verify it can be read back
    write_mem(dut, 0, 123);
    pass &= check_read(dut, 0, 123);

    // Write 456 to address 4 and verify the second location
    write_mem(dut, 4, 456);
    pass &= check_read(dut, 4, 456);

    // Ensure the first memory location was not overwritten
    pass &= check_read(dut, 0, 123);

    if(pass){
        cout << "All data memory tests passed" << endl;
    }
    else{
        cout << "Data memory tests failed" << endl;
    }

    delete dut;
    return pass ? 0 : 1;
}