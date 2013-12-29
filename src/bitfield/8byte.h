#ifndef __8BYTE_H__
#define __8BYTE_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO using uint64_t everywhere for CAN message payload is kind of cute, but
// in actuality a CAN message may have a smaller payload, and it makes all of
// these functions not applicable to other data sizes. It's also fairly
// inefficient on 32-bit platforms. how much work is it to switch vi-firmware
// to using uint8_t*?

/* Public: Reads a subset of bits into a uint64_t.
 *
 * source - the bytes in question.
 * offset - the starting index of the bit field (beginning from 0).
 * numBits - the width of the bit field to extract.
 * big_endian - if the data passed in is little endian, set this to false and it
 *      will be flipped before grabbing the bit field.
 *
 * Bit fields are positioned according to big-endian bit layout, but inside the
 * bit field, values are represented as little-endian. Therefore, to get the bit
 * field, we swap the overall byte order if big_endian == false and
 * use the value we find in the field (assuming the embedded platform is little
 * endian).
 *
 * For example, the bit layout of the value "42" (i.e. 00101010 set at position
 * 14 with length 6 is:
 *
 *     000000000000001010100000000000000000000000000000000000000000000
 *
 * and the same value and position but with length 8 is:
 *
 *     000000000000000010101000000000000000000000000000000000000000000
 *
 * If the architecture where is code is running is little-endian, the input data
 * will be swapped before grabbing the bit field.
 *
 * Examples
 *
 *  uint64_t value = get_bit_field(data, 2, 4);
 *
 * Returns the value of the requested bit field.
 */
uint64_t get_bit_field(uint64_t source, const uint16_t offset,
        const uint16_t bit_count, bool big_endian);

/* Public: Set the bit field in the given data array to the new value.
 *
 * destination - a byte array with size at least offset + bit_count.
 * value - the value to set in the bit field.
 * offset - the starting index of the bit field (beginning from 0).
 * bit_count - the number of bits to set in the data.
 *
 * Returns true if the bit_count is enough to fully represent the value, and
 *      false if it will not fit.
 */
bool set_bit_field(uint64_t* destination, uint64_t value, const uint16_t offset,
        const uint16_t bit_count);

/* Public: Retreive the nth byte out of 8 bytes in a uint64_t.
 *
 * source - the source data to retreive the byte from.
 * byte_index - the index of the byte, starting at 0 and assuming big-endian order.
 *
 * Returns the requested byte from the source bytes.
 */
uint8_t nth_byte(const uint64_t source, const uint16_t byte_index);

#ifdef __cplusplus
}
#endif

#endif // __8BYTE_H__
