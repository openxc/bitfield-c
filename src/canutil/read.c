#include <canutil/read.h>
#include <bitfield/bitfield.h>
#include <bitfield/8byte.h>

static float decode_float(uint64_t raw, float factor, float offset) {
    return raw * factor + offset;
}

float eightbyte_parse_float(uint64_t data, uint8_t bit_offset, uint8_t bit_size,
        float factor, float offset) {
    return decode_float(eightbyte_get_bitfield(data, bit_offset, bit_size,
                true), factor, offset);
}

bool eightbyte_parse_bool(uint64_t data, uint8_t bit_offset, uint8_t bit_size,
        float factor, float offset) {
    float value = eightbyte_parse_float(data, bit_offset, bit_size, factor, offset);
    return value == 0.0 ? false : true;
}

float bitfield_parse_float(const uint8_t source[], const uint16_t source_length,
        const uint8_t bit_offset, const uint8_t bit_size, const float factor,
        const float offset) {
    return decode_float(get_bitfield(source, source_length, bit_offset, bit_size),
            factor, offset);
}

bool bitfield_parse_bool(const uint8_t source[], const uint16_t source_length,
        const uint8_t bit_offset, const uint8_t bit_size, const float factor,
        const float offset) {
    float value = bitfield_parse_float(source, source_length, bit_offset,
            bit_size, factor, offset);
    return value == 0.0 ? false : true;
}
