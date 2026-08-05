#include <iostream>
#include <cstdint>
#include "Vdmem.h"

using namespace std;

// Simulate one complete clock cycle.
// Memory writes occur on the rising edge.
void tick(Vdmem* dut) {
    dut->clk = 0;
    dut->eval();

    dut->clk = 1;
    dut->eval();

    dut->clk = 0;
    dut->eval();
}

// Write a value into data memory.
// funct3 determines the store width:
// 000 = SB
// 001 = SH
// 010 = SW
void write_mem(
    Vdmem* dut,
    uint32_t address,
    uint32_t value,
    uint8_t funct3
) {
    dut->mem_write = 1;
    dut->mem_read = 0;
    dut->funct3 = funct3;
    dut->address = address;
    dut->write_data = value;

    tick(dut);

    dut->mem_write = 0;
    dut->eval();
}

// Read data memory and verify the result.
// The default funct3 value is 010, which means LW.
bool check_read(
    Vdmem* dut,
    uint32_t address,
    uint32_t expected,
    uint8_t funct3 = 0b010
) {
    dut->mem_write = 0;
    dut->mem_read = 1;
    dut->funct3 = funct3;
    dut->address = address;
    dut->eval();

    if (dut->read_data != expected) {
        cout << hex;
        cout << "Fail: address 0x" << address
             << " expected 0x" << expected
             << " got 0x" << dut->read_data
             << endl;
        cout << dec;

        dut->mem_read = 0;
        return false;
    }

    dut->mem_read = 0;
    return true;
}

int main() {
    Vdmem* dut = new Vdmem;
    bool pass = true;

    // Initialize all inputs to known values.
    dut->clk = 0;
    dut->mem_read = 0;
    dut->mem_write = 0;
    dut->funct3 = 0b010;
    dut->address = 0;
    dut->write_data = 0;
    dut->eval();

    // Write 123 to address 0 and verify it can be read back.
    write_mem(dut, 0, 123, 0b010);
    pass &= check_read(dut, 0, 123);

    // Write 456 to address 4 and verify the second location.
    write_mem(dut, 4, 456, 0b010);
    pass &= check_read(dut, 4, 456);

    // Begin with a known word.
    write_mem(dut, 8, 0x11223344, 0b010);
    pass &= check_read(dut, 8, 0x11223344);

    // SB at byte offset 1:
    // 0x11223344 becomes 0x1122AA44.
    write_mem(dut, 9, 0x000000AA, 0b000);
    pass &= check_read(dut, 8, 0x1122AA44);

    // Test SB at all four byte offsets.
    write_mem(dut, 8,  0xAA, 0b000); // offset 0
    write_mem(dut, 9,  0xBB, 0b000); // offset 1
    write_mem(dut, 10, 0xCC, 0b000); // offset 2
    write_mem(dut, 11, 0xDD, 0b000); // offset 3

    // Begin with a known word.
    write_mem(dut, 12, 0x11223344, 0b010);
    pass &= check_read(dut, 12, 0x11223344);

    // SH at offset 0 replaces the lower halfword.
    // 0x11223344 becomes 0x1122AABB.
    write_mem(dut, 12, 0x0000AABB, 0b001);
    pass &= check_read(dut, 12, 0x1122AABB);

    // Reset the word.
    write_mem(dut, 12, 0x11223344, 0b010);

    // SH at offset 2 replaces the upper halfword.
    // 0x11223344 becomes 0xCCDD3344.
    write_mem(dut, 14, 0x0000CCDD, 0b001);
    pass &= check_read(dut, 12, 0xCCDD3344);

    // Full-word read should now return 0xDDCCBBAA.
    pass &= check_read(dut, 8, 0xDDCCBBAA);

    // Ensure another memory location was not overwritten.
    pass &= check_read(dut, 0, 123);

    // Test LBU at all four byte offsets.
    write_mem(dut, 16, 0x80FF7F01, 0b010);

    // LBU offset 0
    pass &= check_read(
        dut,
        16,
        0x00000001,
        0b100
    );

    // LBU offset 1
    pass &= check_read(
        dut,
        17,
        0x0000007F,
        0b100
    );

    // LBU offset 2
    pass &= check_read(
        dut,
        18,
        0x000000FF,
        0b100
    );

    // LBU offset 3
    pass &= check_read(
        dut,
        19,
        0x00000080,
        0b100
    );

        // Test LB at all four byte offsets.
    pass &= check_read(
        dut,
        16,
        0x00000001,
        0b000
    );

    pass &= check_read(
        dut,
        17,
        0x0000007F,
        0b000
    );

    pass &= check_read(
        dut,
        18,
        0xFFFFFFFF,
        0b000
    );

    pass &= check_read(
        dut,
        19,
        0xFFFFFF80,
        0b000
    );

    // Test LHU at both aligned halfword offsets.
    pass &= check_read(
        dut,
        16,
        0x00007F01,
        0b101
    );

    pass &= check_read(
        dut,
        18,
        0x000080FF,
        0b101
    );

    // Test LH at both aligned halfword offsets.
    pass &= check_read(
        dut,
        16,
        0x00007F01,
        0b001
    );

    pass &= check_read(
        dut,
        18,
        0xFFFF80FF,
        0b001
    );

    if (pass) {
        cout << "All data memory tests passed" << endl;
    } else {
        cout << "Data memory tests failed" << endl;
    }

    delete dut;
    return pass ? 0 : 1;
}