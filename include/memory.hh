#pragma once

#include <cstdint>

class Memory {
public:

    // Retrieve 16-bit word from $ADDR -- $(ADDR+1)
    uint16_t ReadW();   // Little endian
    // Retrieve 8-bit byte from $ADDR
    uint8_t ReadB();
    // Retrieve bit value from a byte
    uint8_t GetBit();

    // Attempt to write array of 16-bit data

    // Attempt to write array of 8-bit data

    // Attempt to write a 16-bit word from ADDR -- (ADDR+1)
    // Returns 0 if successful, 1 otherwise
    uint8_t WriteW();
    // Attempt to write an 8-bit byte to ADDR
    // Returns 0 if successful, 1 otherwise
    uint8_t WriteB();
    // Set bit within byte
    void SetBit();
    // Clear bit within byte
    void ClearBit();
    // Toggle bit within byte
    void FlipBit();
};
