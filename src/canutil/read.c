#include <canutil/read.h>
#include <bitfield/bitfield.h>
#include <bitfield/8byte.h>

float eightbyte_parse_float(uint64_t data, uint8_t bit_offset, uint8_t bit_size,
        float factor, float offset) {
    uint64_t raw = eightbyte_get_bit_field(data, bit_offset, bit_size, true);
    return raw * factor + offset;
}

bool eightbyte_parse_bool(uint64_t data, uint8_t bit_offset, uint8_t bit_size,
        float factor, float offset) {
    float value = eightbyte_parse_float(data, bit_offset, bit_size, factor, offset);
    return value == 0.0 ? false : true;
}

float bitfield_parse_float(const uint8_t data[], const uint16_t size,
        const uint8_t bit_offset, const uint8_t bit_size, const float factor,
        const float offset) {
    //TODO
    return 0;
}
