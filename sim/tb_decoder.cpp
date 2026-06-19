#include <iostream>
#include <cstdint>
#include "Vdecoder.h"

using namespace std;

// Checks whether the decoder extracts the correct instruction fields
// from a 32-bit RISC-V instruction
bool check(
    Vdecoder* dut,
    uint32_t instruction,
    uint8_t expected_opcode,
    uint8_t expected_rd,
    uint8_t expected_funct3,
    uint8_t expected_rs1,
    uint8_t expected_rs2,
    uint8_t expected_funct7
)
{
    //Feed instruction in the decoder
    dut->instruction = instruction;

    //Simulate the harware evaluation
    dut-> eval();

    //Verify that all the decoded field matches the expection
    return 
        dut->opcode == expected_opcode &&
        dut->rd == expected_rd &&
        dut->funct3 == expected_funct3 &&
        dut->rs1 == expected_rs1 &&
        dut->rs2 == expected_rs2 &&
        dut->funct7 == expected_funct7;
}

int main(){
    //Create harware model
    Vdecoder* dut = new Vdecoder;
    //Track whether all tests pass
    bool pass = true;

    //Test for R-Type ADD instruction
    pass &= check(
        dut,
        0x007302B3, //Machine code encoding of ADD x5, x6, x7
        0x33,
        5,
        0,
        6,
        7,
        0
    );

    //Test for R-Type SUB instruction
    pass &= check(
        dut,
        0x407302B3,
        0x33,
        5,
        0,
        6,
        7,
        0x20
    );

    if(pass)
        cout << "All decoder tests passed" << endl;
    else
        cout << "Decoder tests failed" << endl;

    delete dut;
    return pass ? 0 : 1;

}