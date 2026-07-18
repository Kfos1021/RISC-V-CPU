#include <iostream>
#include <cstdint>
#include "Valu.h"
using namespace std;

// Runs ALU test case
// a and b are input values
// op is the operation
// expected is the result we expect the ALU to output
bool check(Valu* dut, uint32_t a, uint32_t b, uint8_t op, uint32_t expected) {
    dut->a = a;
    dut->b = b;
    dut->op = op;
    // evaluate logic with inputs
    dut->eval();

    // If the ALU result is wrong
    if (dut->result != expected) {
        cout << "Failed: op= " << (int)op
                  << "a= " << a << " b= " << b
                  << " expected= " << expected
                  << " got= " << dut->result << endl;
        return false;
    }
    return true;
} 


int main(){
    // Create instance of hardware module
    Valu* dut = new Valu;
    bool pass = true;

    // Cycle 0: addi x1, x0, -1
    pass &= check(
        dut,
        0,
        0xFFF00093,
        0xFFFFFFFF,
        1,
        0xFFFFFFFF,
        true
    );
    tick(dut);

    // Cycle 1: addi x2, x0, 1
    pass &= check(
        dut,
        4,
        0x00100113,
        1,
        2,
        1,
        true
    );
    tick(dut);

    // Cycle 2: slt x3, x1, x2
    // Signed comparison: -1 < 1
    pass &= check(
        dut,
        8,
        0x0020A1B3,
        1,
        3,
        1,
        true
    );
    tick(dut);

    // Cycle 3: sltu x4, x1, x2
    // Unsigned comparison: 0xFFFFFFFF is not less than 1
    pass &= check(
        dut,
        12,
        0x0020B233,
        0,
        4,
        0,
        true
    );
    tick(dut);

    // Cycle 4: slti x5, x1, 1
    // Signed comparison: -1 < 1
    pass &= check(
        dut,
        16,
        0x0010A293,
        1,
        5,
        1,
        true
    );
    tick(dut);

    // Cycle 5: sltiu x6, x1, 1
    // Unsigned comparison: 0xFFFFFFFF is not less than 1
    pass &= check(
        dut,
        20,
        0x0010B313,
        0,
        6,
        0,
        true
    );
    tick(dut);

    // Cycle 6: nop
    pass &= check(
        dut,
        24,
        0x00000013,
        0,
        0,
        0,
        true
    );
    tick(dut);


    if(pass){
        cout << "ALL ALU TESTS PASSED" << endl;
    }
    else{
        cout << "ALU TESTS FAILED" << endl;
    }

    delete dut;

    // Return 0 if all tests passed
    return pass ? 0 : 1;
}