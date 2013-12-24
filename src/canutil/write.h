#ifndef __WRITE_H__
#define __WRITE_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t encodeFloat(float value, uint8_t bitPosition, uint8_t bitSize,
                float factor, float offset);

uint64_t encodeBoolean(bool value, uint8_t bitPosition, uint8_t bitSize,
                float factor, float offset);

#ifdef __cplusplus
}
#endif

#endif // __WRITE_H__
