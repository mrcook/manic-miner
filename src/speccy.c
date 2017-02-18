// Speccy library Copyright 2017 Michael R. Cook

#include "externs.h"

uint8_t Speccy_read(int address) {
    if (address < 0 && address >= sizeof(speccy.memory) / sizeof(uint8_t)) {
        exit(-1);
    }

    return speccy.memory[address];
}

void Speccy_write(int address, uint8_t byte) {
    if (address < 0 && address >= sizeof(speccy.memory) / sizeof(uint8_t)) {
        exit(-1);
    }

    speccy.memory[address] = byte;
}

void Speccy_clearScreen() {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    for (int i = 0; i < 6144; i++) {
        speccy.memory[16384 + i] = 0;
    }

//    bzero(speccy.screen, SCREEN_SIZE);
}

uint8_t Speccy_readScreen(int address) {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    return Speccy_read(address);

//    if (address < 16384 && address >= 22528) {
//        exit(-1);
//    }
//
//    return speccy.screen[address - 16384];
}

void Speccy_writeScreen(int address, uint8_t byte) {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    Speccy_write(address, byte);
    return;

//    if (address < 16384 && address >= 22528) {
//        exit(-1);
//    }
//
//    speccy.screen[address - 16384] = byte;
}

uint8_t Speccy_readAttribute(int address) {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    return Speccy_read(address);

//    if (address < 22528 && address >= 23296) {
//        exit(-1);
//    }
//
//    return speccy.attributes[address - 22528];
}

void Speccy_writeAttribute(int address, uint8_t byte) {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    Speccy_write(address, byte);
    return;

//    if (address < 22528 && address >= 23296) {
//        exit(-1);
//    }
//    speccy.attributes[address - 22528] = byte;
}

void Speccy_byteToBits(uint8_t byte, uint8_t *bits) {
    for (int i = 0; i < 8; i++) {
        if (byte & (1 << i)) {
            bits[i] = 1;
        } else {
            bits[i] = 0;
        }
    }
}

void Speccy_convertScreenFormat() {
    int block_addr_offset;
    int address, line, offset;

    for (int block = 0; block < 3; block++) {
        block_addr_offset = block * 2048;
        address = 0;
        line = 0;
        offset = 0;

        for (int byte_row = 0; byte_row < 2048; byte_row += 32) {
            for (int b = 0; b < 32; b++) {
                // FIXME: until we use `speccy.screen` everywhere, we must use normal `speccy.memory`,
                uint8_t bite = speccy.memory[16384 + block_addr_offset + byte_row + b];
                speccy.convertedScreen[block_addr_offset + address] = bite;
                address += 1;
            }

            address += 32 * 7;
            line += 1;

            if (line == 8) {
                line = 0;

                offset += 1;
                address = offset * 32;
            }
        }
    }
}

uint8_t Speccy_readScreenBuffer(int address) {
    if (address < 0 && address >= sizeof(speccy.convertedScreen) / sizeof(uint8_t)) {
        exit(-1);
    }

    return speccy.convertedScreen[address];
}

void split_address(uint16_t addr, uint8_t *msb, uint8_t *lsb) {
    *lsb = (uint8_t) (addr & 0xFF);
    *msb = (uint8_t) (addr >> 8);
}

uint16_t build_address(uint8_t msb, uint8_t lsb) {
    return (msb << 8) | lsb;
}

uint8_t rotl(uint8_t a, uint8_t n) {
    assert (n > 0 && n < 8);
    return (a << n) | (a >> (8 - n));
}

uint8_t rotr(uint8_t a, uint8_t n) {
    assert (n > 0 && n < 8);
    return (a >> n) | (a << (8 - n));
}


//
// Temporary functions
//

uint8_t IN(uint16_t addr) {
    // get keyboard input values
    return 0;
}

void OUT(uint8_t value) {
    // output the sound!
}
