#include "write.h"

uint64_t encodeFloat(float value, uint8_t bitPosition, uint8_t bitSize,
        float factor, float offset) {
    float rawValue = (value - offset) / factor;
    if(rawValue > 0) {
        // round up to avoid losing precision when we cast to an int
        rawValue += 0.5;
    }
    uint64_t result = 0;
    setBitField(&result, (uint64_t)rawValue, bitPosition, bitSize);
    return result;
}

uint64_t encodeBoolean(bool value, uint8_t bitPosition, uint8_t bitSize,
        float factor, float offset) {
    return encodeFloat(value, offset, factor, bitPosition, bitSize);
}
