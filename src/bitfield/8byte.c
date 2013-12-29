#include <bitfield/bitfield.h>
#include <bitfield/8byte.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>

#define EIGHTBYTE_BIT (8 * sizeof(uint64_t))

uint64_t bitmask(const uint8_t bit_count) {
    return (((uint64_t)0x1) << bit_count) - 1;
}

static uint16_t bits_to_bytes(uint32_t bits) {
    uint8_t byte_count = bits / CHAR_BIT;
    if(bits % CHAR_BIT != 0) {
        ++byte_count;
    }
    return byte_count;
}

uint64_t get_bit_field(uint64_t source, const uint16_t startBit,
        const uint16_t bit_count, bool big_endian) {
    uint8_t result[8] = {0};
    if(!big_endian) {
        source = __builtin_bswap64(source);
    }
    copyBitsRightAligned((const uint8_t*)&source, sizeof(source), startBit,
            bit_count, result, sizeof(result));
    uint64_t int_result = 0;

    if(!big_endian) {
        // we need to swap the byte order of the array to get it into a
        // uint64_t, but it's been right aligned so we have to be more careful
        for(int i = 0; i < bits_to_bytes(bit_count); i++) {
            int_result |= result[bits_to_bytes(bit_count) - i - 1] << (CHAR_BIT * i);
        }
    } else {
        int_result = *(uint64_t*)result;
    }
    return int_result;
}

bool set_bit_field(uint64_t* destination, uint64_t value, const uint16_t offset,
        const uint16_t bit_count) {
    if(value > bitmask(bit_count)) {
        return false;
    }

    int shiftDistance = EIGHTBYTE_BIT - offset - bit_count;
    value <<= shiftDistance;
    *destination &= ~(bitmask(bit_count) << shiftDistance);
    *destination |= value;
    return true;
}

uint8_t nth_byte(const uint64_t source, const uint16_t byte_index) {
    return (source >> (EIGHTBYTE_BIT - ((byte_index + 1) * CHAR_BIT))) & 0xFF;
}

