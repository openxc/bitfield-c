#ifndef __8BYTE_H__
#define __8BYTE_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Public: Reads a subset of bits into a uint64_t.
 *
 * source - the bytes in question.
 * offset - the starting index of the bit field (beginning from 0).
 * bit_count - the width of the bit field to extract.
 * data_is_big_endian - if the data passed in is little endian, set this to false and it
 *      will be flipped before grabbing the bit field.
 *
 * Bit fields are positioned according to big-endian bit layout.
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
 *  uint64_t value = get_bitfield(data, 2, 4);
 *
 * Returns the value of the requested bit field, right aligned in a uint64_t.
 */
uint64_t eightbyte_get_bitfield(uint64_t source, const uint16_t offset,
        const uint16_t bit_count, const bool data_is_big_endian);

/* Public: Return a single nibble from the payload, with range checking.
 *
 * source - the source payload.
 * nibble_index - the index of the nibble to retreive. The leftmost nibble is
 *      index 0.
 * data_is_big_endian - if the data passed in is little endian, set this to false and it
 *      will be flipped before grabbing the bit field.
 *
 * Returns the retreived nibble, right aligned in a uint8_t.
 */
uint8_t eightbyte_get_nibble(const uint64_t source, const uint8_t nibble_index,
        const bool data_is_big_endian);

/* Public: Return a single byte from the payload, with range checking.
 *
 * source - the source byte array.
 * byte_index - the index of the byte to retreive. The leftmost byte is index 0.
 * data_is_big_endian - if the data passed in is little endian, set this to false and it
 *      will be flipped before grabbing the bit field.
 *
 * Returns the retreived byte.
 */
uint8_t eightbyte_get_byte(const uint64_t source, const uint8_t byte_index,
        const bool data_is_big_endian);

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
bool eightbyte_set_bitfield(uint64_t value,
        const uint16_t offset, const uint16_t bit_count, uint64_t* destination);

/* Private: Determine the index of the last bit used.
 */
uint8_t find_end_bit(const uint16_t num_bits);

#ifdef __cplusplus
}
#endif

#endif // __8BYTE_H__
