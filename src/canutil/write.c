#include <canutil/write.h>
#include <bitfield/bitfield.h>
#include <bitfield/8byte.h>

uint64_t float_to_fixed_point(const float value, const float factor,
        const float offset) {
    float raw = (value - offset) / factor;
    if(raw > 0) {
        // round up to avoid losing precision when we cast to an int
        // TODO do we need a way to encode an int back to a signal without any
        // rounding?
        raw += 0.5;
    }
    return (uint64_t)raw;
}

uint64_t eightbyte_encode_float(float value, uint8_t bit_offset, uint8_t bit_size,
        float factor, float offset) {
    uint64_t result = 0;
    if(!eightbyte_set_bitfield(float_to_fixed_point(value, factor, offset),
                bit_offset, bit_size, &result)) {
        // debug("%f will not fit in a %d bit field", value, bit_size);
    }
    return result;
}

uint64_t eightbyte_encode_bool(const bool value, const uint8_t bit_offset,
        const uint8_t bit_size) {
    return eightbyte_encode_float(value, bit_offset, bit_size, 1.0, 0);
}

bool bitfield_encode_float(const float value, const uint8_t bit_offset,
        const uint8_t bit_size, const float factor, const float offset,
        uint8_t destination[], const uint8_t destination_length) {
    if(!set_bitfield(float_to_fixed_point(value, factor, offset), bit_offset,
                bit_size, destination, destination_length)) {
        // debug("%f will not fit in a %d bit field", value, bit_size);
        return false;
    }
    return true;
}

bool bitfield_encode_bool(const bool value, const uint8_t bit_offset,
        const uint8_t bit_size, uint8_t destination[],
        const uint16_t destination_length) {
    return bitfield_encode_float(value, bit_offset, bit_size, 1.0, 0,
            destination, destination_length);
}
