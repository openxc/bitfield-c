#include <bitfield/bitfield.h>

float parseFloat(uint64_t data, uint8_t bitPosition, uint8_t bitSize,
        float factor, float offset) {
    uint64_t rawValue = getBitField(data, bitPosition,
            bitSize, true);
    return rawValue * factor + offset;
}

bool parseBoolean(uint64_t data, uint8_t bitPosition, uint8_t bitSize,
        float factor, float offset) {
    float value = parseFloat(data, bitPosition, bitSize, factor, offset);
    return value == 0.0 ? false : true;
}
