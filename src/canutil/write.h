#ifndef __WRITE_H__
#define __WRITE_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Public: Encode a floating point number into a fixed point, fixed bit width
 *      field in a bit array.
 *
 * value - the floating point value to encode.
 * bit_offset - the starting point for the encoded bits in the returned value.
 * bit_size - The max width of the field in the resulting bit array. If bit_size
 *      isn't big enough to store the fixed point version of the value, the
 *      bitfeld will *not* be set. TODO some error reporting would be nice.
 * factor - a factor used to transform from floating to fixed point before
 *      encoding. Use 1.0 for no factor.
 * offset - an offset used to transform from floating to fixed point before
 *      encoding. Use 0 for no offset.
 *
 * Returns a big-endian uint64_t with the value encoded as a bitfield.
 */
uint64_t eightbyte_encode_float(float value, uint8_t bit_offset,
        uint8_t bit_size, float factor, float offset);

uint64_t float_to_fixed_point(const float value, const float factor,
        const float offset);

bool bitfield_encode_float(const float value, const uint8_t bit_offset,
        const uint8_t bit_size, const float factor, const float offset,
        uint8_t destination[], const uint8_t destination_length);

/* Public: Encode a boolean into fixed bit width field in a bit array.
 *
 * value - the boolean value to encode - true will be 1, false will be 0.
 * bit_offset - the starting point for the encoded bits in the returned value.
 * bit_size - The max width of the field in the resulting bit array. If bit_size
 *      isn't big enough to store the fixed point version of the value, the
 *      bitfeld will *not* be set. TODO some error reporting would be nice.
 *
 * Returns a big-endian uint64_t with the value encoded as a bitfield.
 */
uint64_t eightbyte_encode_bool(const bool value, const uint8_t bit_offset,
        const uint8_t bit_size);

bool bitfield_encode_bool(const bool value, const uint8_t bit_offset, const
        uint8_t bit_size, uint8_t destination[],
        const uint16_t destination_length);

#ifdef __cplusplus
}
#endif

#endif // __WRITE_H__
