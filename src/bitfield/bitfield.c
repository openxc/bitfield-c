#include <bitfield/bitfield.h>
#include <limits.h>
#include <string.h>
#include <stddef.h>

uint8_t get_nibble(const uint8_t source[], const uint8_t source_length,
                const uint8_t nibble_index) {
    uint8_t byte_index = nibble_index / 2;
    uint8_t result = get_byte(source, source_length, byte_index);
    if(nibble_index % 2 == 0) {
        result >>= NIBBLE_SIZE;
    }
    result &= bitmask(NIBBLE_SIZE);
    return result;
}

uint8_t get_byte(const uint8_t source[], const uint8_t source_length,
        const uint8_t byte_index) {
    if(byte_index < source_length) {
        return source[byte_index];
    }
    return 0;
}
