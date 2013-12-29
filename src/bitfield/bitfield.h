#ifndef __BITFIELD_H__
#define __BITFIELD_H__

#include <stdint.h>
#include <stdbool.h>

#define NIBBLE_SIZE (CHAR_BIT / 2)

#ifdef __cplusplus
extern "C" {
#endif

/* Public: Return a single nibble from the byte array, with range checking.
 *
 * source - the source byte array.
 * source_length - the total length of the source array.
 * nibble_index - the index of the nibble to retreive. The leftmost nibble is
 *      index 0.
 *
 * Returns the retreived nibble, right aligned in a uint8_t.
 */
uint8_t get_nibble(const uint8_t source[], const uint8_t source_length,
                const uint8_t nibble_index);

/* Public: Return a single byte from the byte array, with range checking.
 *
 * source - the source byte array.
 * source_length - the total length of the source array.
 * byte_index - the index of the byte to retreive. The leftmost byte is index 0.
 *
 * Returns the retreived byte.
 */
uint8_t get_byte(const uint8_t source[], const uint8_t source_length,
        const uint8_t byte_index);

/* Public: Copy a range of bits from one bit array to another.
 *
 * The range does not need to be byte aligned, and the source and destination do
 * not have to be the same size (as long as the desitnation has enough room to
 * fit the range).
 *
 * A bit array with regards to this function always has the leftmost bit in byte
 * 0, i.e. bit index is the leftmost bit of byte 0. Endianness does not matter.
 *
 * For example:
 *
 *      uint8_t source[4] = {0x11, 0x22, 0x33, 0x44};
 *      uint8_t destination[4] = {0};
 *      copy_bits(source, sizeof(source), 8, 8, destination,
 *              sizeof(destination), 0);
 *      // destination[0] == 0x22
 *      // destination[1] == 0x0
 *      // destination[2] == 0x0
 *      // destination[3] == 0x0
 *
 * Thanks to
 * http://stackoverflow.com/questions/3534535/whats-a-time-efficient-algorithm-to-copy-unaligned-bit-arrays
 * for the implementation of the algorithm.
 *
 * source_origin - the source array.
 * source_length - the total length of the source array in bytes,
 *      for range checking.
 * source_offset - an offset in bits to start the copy from the source array.
 *      Specify 0 to start from source_origin.
 * bit_count - the number of bits to copy.
 * destination_origin - the destination array.
 * desitnation_length - the total length of the destination array in bytes,
 *      for range checking.
 * destination_offset - an offset in bits to start placing the copied range into
 *      the destination array. Specify 0 to start from the beginning of the
 *      destination. If you are copying a range not aligned on a byte, you
 *      probably want to set this to a positive offset to right the resulting
 *      bits in the destination.
 *
 * Returns true if the copy was successful and false if the range exceeded the
 * size of the source or destination, or if the range size negative or 0.
 */
bool copy_bits(const uint8_t* source_origin, const uint16_t source_length,
        const uint16_t source_offset, uint16_t bit_count,
        uint8_t* destination_origin, const uint16_t destination_length,
        const uint16_t destination_offset);

/* Public: Copy a range of bits from one array to another, right aligning the
 * result.
 *
 * This is mostly useful if you want to cast the result to an integer type
 * instead of a byte array.
 *
 * For example:
 *
 *      uint8_t source[4] = {0x11, 0x22, 0x33, 0x44};
 *      uint8_t destination[4] = {0};
 *      copy_bits_right_aligned(source, sizeof(source), 8, 8, destination,
 *              sizeof(destination));
 *      // destination[0] == 0x0
 *      // destination[1] == 0x0
 *      // destination[2] == 0x0
 *      // destination[3] == 0x22
 *
 *      int value = (int)destination;
 *      // value == 0x22 == 32
 *
 * The arguments are the same as copy_bits, but without the destination_offset
 * option - that's set automatically to right align the result.
 *
 * Returns true if the copy was successful and false if the range exceeded the
 * size of the source or destination, or if the range size negative or 0.
 */
bool copy_bits_right_aligned(const uint8_t source[], const uint16_t source_length,
                const uint16_t offset, const uint16_t bit_count,
                uint8_t* destination, const uint16_t destination_length);

#ifdef __cplusplus
}
#endif

#endif // __BITFIELD_H__
