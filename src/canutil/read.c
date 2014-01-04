#include <canutil/read.h>
#include <bitfield/bitfield.h>
#include <bitfield/8byte.h>

float bitfield_parse_float(uint64_t data, uint8_t bit_offset, uint8_t bit_size,
        float factor, float offset) {
    uint64_t raw = get_bit_field(data, bit_offset,
            bit_size, true);
    return raw * factor + offset;
}

bool bitfield_parse_bool(uint64_t data, uint8_t bit_offset, uint8_t bit_size,
        float factor, float offset) {
    float value = bitfield_parse_float(data, bit_offset, bit_size, factor, offset);
    return value == 0.0 ? false : true;
}
