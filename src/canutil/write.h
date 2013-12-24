#ifndef __WRITE_H__
#define __WRITE_H__

#include <stdint.h>
#include <stdbool.h>

uint64_t encodeFloat(float value, float offset, float factor, uint8_t bitPosition,
        uint8_t bitSize);

uint64_t encodeBoolean(bool value, float offset, float factor,
                uint8_t bitPosition, uint8_t bitSize);

#endif // __WRITE_H__
