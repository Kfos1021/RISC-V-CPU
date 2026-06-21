#include <iostream>
#include <cstdint>
#include "Vcontrol.h"

using namespace std;

//Applies inputs to the control unit and checks
//whether all output match the expected values
bool check(
    Vcontrol* dut,

    uint8_t opcode,
    uint8_t funct3,
    uint8_t funct7,

    bool expected_reg_write,
    bool expected_alu_src,
    bool expected_mem_read,
    bool expected_mem_write,
    bool expected_mem_to_reg,
    bool expected_branch,
    uint8_t expected_alu_op
)
{
    //Apply the intruction fields to Device Under Test (DUT)
    dut->opcode = opcode;
    dut->funct3 = funct3;
    dut->funct7 = funct7;

    //Evaluate combinational logic
    dut->eval();

    //Verify the control signals
    return
        dut->reg_write == expected_reg_write &&
        dut->alu_src == expected_alu_src &&
        dut->mem_read == expected_mem_read &&
        dut->mem_write == expected_mem_write &&
        dut->mem_to_reg == expected_mem_to_reg &&
        dut->branch == expected_branch &&
        dut->alu_op == expected_alu_op;
}

int main(){
    //Create DUT
    Vcontrol* dut = new Vcontrol;

    //Track the overall test status
    bool pass = true;

    //ADD test
    //add x, y, z
    pass &= check(
        dut,

        0x33,   //opcode
        0x0,    //funct3   
        0x00,
        
        true,   //reg_write
        false,  //alu_src
        false,  //mem_read
        false,  //mem_write
        false,  //mem_to_reg
        false,  //branch
        0       //ADD for address calculation
    );

    //SUB test
    // sub x, y, z
    pass &= check(
        dut,

        0x33,
        0x0,
        0x20,

        true,   //reg_write
        false,  //alu_src
        false,  //mem_read
        false,  //mem_write
        false,  //mem_to_reg
        false,  //branch
        1       //ADD for address calculation
    );

    //LW (Load Word) test
    //lw rd, offset(rs1)
    pass &= check(
        dut,

        0x03,
        0,
        0,
    
        true,   //reg_write
        true,   //alu_src
        true,   //mem_read
        false,  //mem_write
        true,   //mem_to_reg
        false,  //branch
        0       //ADD for address calculation
    );

    //SW (Store Word) test
    //sw rs2, offset(rs1)
    pass &= check(
        dut,

        0x23,
        0,
        0,

        false,  //reg_write
        true,   //alu_src
        false,  //mem_read
        true,   //mem_write
        false,  //mem_to_reg
        false,  //branch
        0       //ADD for address calculation
    );

    //BEQ (Branch if Equal)
    //beq rs1, rs2, label
    pass &= check(
        dut,
        
        0x63,
        0,
        0,

        false,  //reg_write
        false,  //alu_src
        false,  //mem_read
        false,  //mem_write
        false,  //mem_to_reg
        true,   //branch
        1       //SUB for comparison
    );

    if(pass){
        cout << "All control tests passed" << endl;
    }
    else{
        cout << "Control tests failed" << endl;
    }

    delete dut;
    //Return success/failure to Makefile
    return pass ? 0 : 1;
}