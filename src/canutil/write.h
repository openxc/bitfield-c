#ifndef __WRITE_H__
#define __WRITE_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t bitfield_encode_float(float value, uint8_t bit_offset, uint8_t bit_size,
                float factor, float offset);

uint64_t bitfield_encode_bool(bool value, uint8_t bit_offset, uint8_t bit_size,
                float factor, float offset);

#ifdef __cplusplus
}
#endif

#endif // __WRITE_H__
