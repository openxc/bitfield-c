#include "write.h"
#include <bitfield/bitfield.h>

uint64_t bitfield_encode_float(float value, uint8_t bit_offset, uint8_t bit_size,
        float factor, float offset) {
    float raw = (value - offset) / factor;
    if(raw > 0) {
        // round up to avoid losing precision when we cast to an int
        raw += 0.5;
    }
    uint64_t result = 0;
    set_bit_field(&result, (uint64_t)raw, bit_offset, bit_size);
    return result;
}

uint64_t bitfield_encode_bool(bool value, uint8_t bit_offset, uint8_t bit_size,
        float factor, float offset) {
    return bitfield_encode_float(value, offset, factor, bit_offset, bit_size);
}
