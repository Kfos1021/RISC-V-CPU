#include <iostream>
#include <cstdint>
#include "Vregfile.h"

using namespace std;

// Tick function
// Creates clock transition
void tick(Vregfile* dut){
    dut->clk = 0;
    dut->eval();

    dut->clk = 1;
    dut->eval();
    
    dut->clk = 0;
    dut->eval();

}

// Read registers and verify the values
bool check_read(Vregfile* dut, uint8_t rs1, uint8_t rs2, uint32_t expected1, uint32_t expected2){
    // Select the registers to read
    dut->rs1 = rs1;
    dut->rs2 = rs2;
    dut->eval();

    if (dut->read_data1 != expected1 || dut->read_data2 != expected2){
        cout << "FAIL: rs1=x" << (int)rs1
             << " expected " << expected1
             << " got " << dut->read_data1
             << " | rs2=x" << (int)rs2
             << " expected " << expected2
             << " got " << dut->read_data2 << endl;
        return false;
    }

    return true;
}

// Write value into the register file
void write_reg(Vregfile* dut, uint8_t rd, uint32_t value){
    dut->we = 1;
    dut->rd = rd;
    dut->write_data = value;
    tick(dut);
    dut->we = 0;
    dut->eval();
}


int main(){
    // Create hardware
    Vregfile* dut = new Vregfile;
    bool pass = true;

    dut->clk = 0;
    dut->we = 0;
    dut->rs1 = 0;
    dut->rs2 = 0;
    dut->rd = 0;
    dut->write_data = 0;
    dut->eval();

    pass &= check_read(dut, 0, 0, 0, 0);

    write_reg(dut, 5, 123);
    pass &= check_read(dut, 5, 0 , 123, 0);

    write_reg(dut, 10, 456);
    pass &= check_read(dut, 5, 10, 123, 456);

    write_reg(dut, 0, 999);
    pass &= check_read(dut, 0, 10, 0, 456);

    if(pass){
        cout << "All Regfile tests passed" << endl;
    }
    else{
        cout << "Regfile tests failed" << endl;
    }

    delete dut;
    return pass ? 0 : 1;
}
