#include <iostream>
#include <cstdint>
#include <random>
#include <string>
#include "Valu.h"
#include "verilated.h"

using namespace std;

// Runs ALU test case
// a and b are input values
// op is the operation
// expected is the result we expect the ALU to output
bool check(Valu* dut, uint32_t a, uint32_t b, uint8_t op, uint32_t expected) {
    dut->a = a;
    dut->b = b;
    dut->op = op;    // evaluate logic with inputs
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

uint32_t reference_alu(
    uint32_t a,
    uint32_t b,
    uint8_t alu_op
) {
    switch (alu_op) {
        case 0: // ADD
            return a + b;

        case 1: // SUB
            return a - b;

        case 2: // AND
            return a & b;

        case 3: // OR
            return a | b;

        case 4: // XOR
            return a ^ b;

        case 5: // SLT
        return static_cast<int32_t>(a) < static_cast<int32_t>(b) ? 1u : 0u;

        case 6: // SLL
            return a << (b & 0x1F);

        case 7: // SRL
            return a >> (b & 0x1F);

        case 8: // SRA
            return static_cast<uint32_t>(
                static_cast<int32_t>(a) >> (b & 0x1F)
            );

        case 9: // SLTU
            return a < b ? 1u : 0u;

        default:
            return 0;
    }
}

bool test_random_operation(
    Valu* dut,
    uint8_t alu_op,
    const string& operation_name,
    mt19937& generator,
    int number_of_tests
) {
    uniform_int_distribution<uint32_t> distribution(
        0,
        UINT32_MAX
    );

    for (int test_number = 0;
         test_number < number_of_tests;
         test_number++) {

        uint32_t a = distribution(generator);
        uint32_t b = distribution(generator);

        dut->a = a;
        dut->b = b;
        dut->op = alu_op;           
        dut->eval();

        uint32_t expected = reference_alu(a, b, alu_op);
        uint32_t actual = dut->result;

        if (actual != expected) {
            cout << "Randomized " << operation_name
                 << " test failed" << endl;

            cout << hex;
            cout << "a        = 0x" << a << endl;
            cout << "b        = 0x" << b << endl;
            cout << "expected = 0x" << expected << endl;
            cout << "actual   = 0x" << actual << endl;
            cout << dec;

            cout << "Test number: " << test_number << endl;

            return false;
        }
    }

    cout << operation_name
         << ": passed "
         << number_of_tests
         << " randomized tests"
         << endl;

    return true;
}

bool test_edge_cases(Valu* dut) {
    struct TestCase {
        uint32_t a;
        uint32_t b;
        uint8_t alu_op;
        const char* name;
    };

    TestCase tests[] = {
        // ADD overflow and basic cases
        {0x00000000, 0x00000000, 0, "ADD zero"},
        {0xFFFFFFFF, 0x00000001, 0, "ADD overflow"},
        {0x7FFFFFFF, 0x00000001, 0, "ADD signed boundary"},

        // SUB underflow
        {0x00000000, 0x00000001, 1, "SUB underflow"},
        {0x80000000, 0x00000001, 1, "SUB signed boundary"},

        // Logic
        {0xAAAAAAAA, 0x55555555, 2, "AND alternating bits"},
        {0xAAAAAAAA, 0x55555555, 3, "OR alternating bits"},
        {0xAAAAAAAA, 0x55555555, 4, "XOR alternating bits"},

        // Shifts
        {0x00000001, 31, 5, "SLL by 31"},
        {0x80000000, 31, 6, "SRL by 31"},
        {0x80000000, 31, 7, "SRA negative by 31"},
        {0x80000000, 0,  7, "SRA by zero"},
        {0x00000001, 32, 5, "SLL masked shift amount"},
        {0x00000008, 34, 6, "SRL masked shift amount"},

        // Signed comparison
        {0xFFFFFFFF, 0x00000001, 8, "SLT negative less than positive"},
        {0x00000001, 0xFFFFFFFF, 8, "SLT positive versus negative"},
        {0x80000000, 0x7FFFFFFF, 8, "SLT signed extremes"},

        // Unsigned comparison
        {0xFFFFFFFF, 0x00000001, 9, "SLTU maximum versus one"},
        {0x00000001, 0xFFFFFFFF, 9, "SLTU one versus maximum"},
        {0x00000000, 0x00000000, 9, "SLTU equal values"}
    };

    bool pass = true;

    for (const TestCase& test : tests) {
        dut->a = test.a;
        dut->b = test.b;
        dut->op = test.alu_op;
        dut->eval();

        uint32_t expected =
            reference_alu(test.a, test.b, test.alu_op);

        if (dut->result != expected) {
            cout << "Edge-case test failed: "
                 << test.name << endl;

            cout << hex;
            cout << "a        = 0x" << test.a << endl;
            cout << "b        = 0x" << test.b << endl;
            cout << "expected = 0x" << expected << endl;
            cout << "actual   = 0x"
                 << static_cast<uint32_t>(dut->result)
                 << endl;
            cout << dec;

            pass = false;
        }
    }

    if (pass) {
        cout << "All directed edge cases passed" << endl;
    }

    return pass;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    Valu* dut = new Valu;

    bool pass = true;

    // Fixed seed means failures are reproducible.
    mt19937 generator(12345);

    constexpr int tests_per_operation = 1000;

    pass &= test_edge_cases(dut);

    pass &= test_random_operation(
        dut, 0, "ADD", generator, tests_per_operation
    );

    pass &= test_random_operation(
        dut, 1, "SUB", generator, tests_per_operation
    );

    pass &= test_random_operation(
        dut, 2, "AND", generator, tests_per_operation
    );

    pass &= test_random_operation(
        dut, 3, "OR", generator, tests_per_operation
    );

    pass &= test_random_operation(
        dut, 4, "XOR", generator, tests_per_operation
    );

    pass &= test_random_operation(
        dut, 5, "SLT", generator, tests_per_operation
    );
   
    pass &= test_random_operation(
        dut, 6, "SLL", generator, tests_per_operation
    );
   
    pass &= test_random_operation(
        dut, 7, "SRL", generator, tests_per_operation
    );
   
    pass &= test_random_operation(
        dut, 8, "SRA", generator, tests_per_operation
    );
   
    pass &= test_random_operation(
        dut, 9, "SLTU", generator, tests_per_operation
    );

    if (pass) {
        cout << "All ALU tests passed" << endl;
    } else {
        cout << "ALU tests failed" << endl;
    }

    delete dut;

    return pass ? 0 : 1;
}