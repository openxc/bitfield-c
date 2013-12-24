#ifndef __READ_H__
#define __READ_H__

#include <stdint.h>
#include <stdbool.h>

float parseFloat(uint64_t data, uint8_t bitPosition, uint8_t bitSize,
        float factor, float offset);

bool parseBoolean(uint64_t data, uint8_t bitPosition, uint8_t bitSize,
        float factor, float offset);

#endif // __READ_H__
