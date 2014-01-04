#include <canutil/write.h>
#include <bitfield/bitfield.h>
#include <bitfield/8byte.h>

uint64_t eightbyte_encode_float(float value, uint8_t bit_offset, uint8_t bit_size,
        float factor, float offset) {
    float raw = (value - offset) / factor;
    if(raw > 0) {
        // round up to avoid losing precision when we cast to an int
        // TODO do we need a way to encode an int back to a signal without any
        // rounding?
        raw += 0.5;
    }
    uint64_t result = 0;
    if(!set_bit_field(&result, (uint64_t)raw, bit_offset, bit_size)) {
        // debug("%f will not fit in a %d bit field", value, bit_size);
    }
    return result;
}

uint64_t eightbyte_encode_bool(const bool value, const uint8_t bit_offset,
        const uint8_t bit_size) {
    return eightbyte_encode_float(value, bit_offset, bit_size, 1.0, 0);
}

bool bitfield_encode_float(float value, uint8_t bit_offset,
        uint8_t bit_size, float factor, float offset, uint8_t destination[]) {
    // TODO
    return 0;
}
