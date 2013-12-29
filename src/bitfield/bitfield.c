#include <bitfield/bitfield.h>
#include <limits.h>
#include <string.h>
#include <stddef.h>

#define NIBBLE_SIZE (CHAR_BIT / 2)

uint64_t bitmask(const uint8_t numBits) {
    return (((uint64_t)0x1) << numBits) - 1;
}

static uint16_t bitsToBytes(uint32_t bits) {
    uint8_t byte_count = bits / CHAR_BIT;
    if(bits % CHAR_BIT != 0) {
        ++byte_count;
    }
    return byte_count;
}

uint64_t getBitField(uint64_t data, const uint16_t startBit,
        const uint16_t numBits, bool bigEndian) {
    uint8_t result[8] = {0};
    if(!bigEndian) {
        data = __builtin_bswap64(data);
    }
    copyBitsRightAligned((const uint8_t*)&data, sizeof(data), startBit, numBits,
            result, sizeof(result));
    uint64_t int_result = 0;

    if(!bigEndian) {
        // we need to swap the byte order of the array to get it into a
        // uint64_t, but it's been right aligned so we have to be more careful
        for(int i = 0; i < bitsToBytes(numBits); i++) {
            int_result |= result[bitsToBytes(numBits) - i - 1] << (CHAR_BIT * i);
        }
    } else {
        int_result = *(uint64_t*)result;
    }
    return int_result;
}

/**
 * TODO it would be nice to have a warning if you call with this a value that
 * won't fit in the number of bits you've specified it should use.
 */
void setBitField(uint64_t* data, uint64_t value, const uint16_t startPos,
        const uint16_t numBits) {
    int shiftDistance = 64 - startPos - numBits;
    value <<= shiftDistance;
    *data &= ~(bitmask(numBits) << shiftDistance);
    *data |= value;
}

uint8_t nthByte(const uint64_t source, const uint16_t byteNum) {
    return (source >> (64 - ((byteNum + 1) * CHAR_BIT))) & 0xFF;
}

uint8_t getNibble(const uint8_t nibble_index, const uint8_t data[],
        const uint8_t length) {
    uint8_t byte_index = nibble_index / 2;
    uint8_t result;
    if(byte_index < length) {
        result = data[byte_index];
        if(nibble_index % 2 == 0) {
            result >>= NIBBLE_SIZE;
        }
    }
    result &= bitmask(NIBBLE_SIZE);
    return result;
}

uint8_t getByte(const uint8_t byte_index, const uint8_t data[],
        const uint8_t length) {
    if(byte_index < length) {
        return data[byte_index];
    }
    return 0;
}
