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

    // Tests adding 10 + 5, should equal 15
    pass &= check(dut, 10, 5, 0, 15);

    // Tests subtracting 10 - 5, should equal 5
    pass &= check(dut, 10, 5, 1, 5);

    // Tests AND
    pass &= check(dut, 0b1100, 0b1010, 2, 0b1000);

    // Tests OR
    pass &= check(dut, 0b1100, 0b1010, 3, 0b1110);

    // Tests XOR
    pass &= check(dut, 0b1100, 0b1010, 4, 0b0110);

    // Tests SLT
    pass &= check(dut, 3, 7, 5, 1);

    // Tests SLT
    pass &= check(dut, 7, 3, 5, 0);

    // Tests Shift Right Logical
    pass &= check(dut, 1, 3, 6, 8);

    // Tests Shift Right Logical
    pass &= check(dut, 8, 1, 7, 4);

    // SLL: 1 << 3 = 8
    pass &= check(dut, 1, 3, 6, 8);

    // SRL: 8 >> 1 = 4
    pass &= check(dut, 8, 1, 7, 4);

    // SRA: -16 >>> 2 = -4
    pass &= check(dut, 0xFFFFFFF0, 2, 8, 0xFFFFFFFC); // SRA: -16 >>> 2 = -4

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