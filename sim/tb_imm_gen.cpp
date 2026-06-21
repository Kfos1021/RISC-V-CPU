#include <iostream>
#include <cstdint>
#include "Vimm_gen.h"

using namespace std;

//Runs one immediate-generator test case
//Returns true if the generated immediate matches the expected value
bool check(Vimm_gen* dut, uint32_t instruction, uint32_t expected_imm){
    //Apply the instruction to Device Under Test (DUT)
    dut->instruction = instruction;
    dut->eval();

    //Verify generated immediate matches expectations
    if(dut->imm != expected_imm){
        cout << "Fail: instruction = 0x" << hex << instruction 
             << " expected imm = 0x" << expected_imm
             << " got imm = 0x" << dut->imm << dec << endl;
        return false;
    }
    return true;
}

int main(){
    //Create instance of immediate generator
    Vimm_gen* dut = new Vimm_gen;
    bool pass = true;

    //Test I-type immediate extraction
    pass &= check(dut, 0x00A30293, 10);
    //Test S-type immediate extraction
    pass &= check(dut, 0x00832283, 8);
    //Test B-type immediate extraction
    pass &= check(dut, 0x00532623, 12);
    //Additional I-type immedate test
    pass &= check(dut, 0x00628463, 8);

    //Print overall test result
    if(pass){
        cout << "All immediate generator tests passed" << endl;
    }
    else{
        cout << "Immediate generator tests failed" << endl;
    }

    delete dut;
    return pass ? 0 : 1;
}