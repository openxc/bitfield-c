#ifndef __BITFIELD_H__
#define __BITFIELD_H__

#include <stdint.h>
#include <stdbool.h>

#define NIBBLE_SIZE (CHAR_BIT / 2)

#ifdef __cplusplus
extern "C" {
#endif

/* Public: Reads a subset of bits into a uint64_t, right aligned so they may be
 * interpreted as a number.
 *
 * source - the bytes in question.
 * source_size - the number of bytes in the source.
 * offset - the starting index of the bit field (beginning from 0).
 * bit_count - the width of the bit field to extract. This must be less than or
 *      equal to 64.
 *
 * Bit fields are positioned according to big-endian bit layout and the data is
 * swapped automatically as necessary depending on the compiled architecture.
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
 * Examples
 *
 *  uint64_t value = get_bitfield(data, data_size, 2, 4);
 *
 * Returns the value of the requested bit field, right aligned in a uint64_t.
 */
uint64_t get_bitfield(const uint8_t source[], const uint8_t source_length,
                const uint16_t offset, const uint16_t bit_count);

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

/* Public: Copy a range of bytes from one byte array to another.
 *
 * The source and destination do not have to be the same size (as long as the
 * desitnation has enough room to fit the range).
 *
 * source_origin - the source array.
 * source_length - the total length of the source array in bytes,
 *      for range checking.
 * source_offset - a byte offset to start the copy from the source array.
 *      Specify 0 to start from source_origin.
 * byte_count - the number of bytes to copy.
 * destination_origin - the destination array.
 * desitnation_length - the total length of the destination array in bytes,
 *      for range checking.
 * destination_offset - an offset in bytes to start placing the copied range into
 *      the destination array. Specify 0 to start from the beginning of the
 *      destination.
 *
 * Returns true if the copy was successful and false if the range exceeded the
 * size of the source or destination, or if the range size negative or 0.
 */
bool copy_bytes_right_aligned(const uint8_t source[], const uint16_t source_length,
                const uint16_t offset, const uint16_t byte_count,
                uint8_t* destination, const uint16_t destination_length);

/* Public: Set the a nibble in the given data array to the new value.
 *
 * nibble_index - the index of the nibble to retreive. The leftmost nibble is
 *      index 0.
 * value - the value to set in the bit field.
 * destination - the destination array.
 * destination_length - the total length of the destination array in bytes,
 *      for range checking.
 *
 * Returns true if the bit_count is enough to fully represent the value, and
 *      false if it will not fit.
 */
bool set_nibble(const uint16_t nibble_index, const uint8_t value,
                uint8_t* destination, const uint16_t destination_length);

/* Public: Set the bit field in the given data array to the new value.
 *
 * value - the value to set in the bit field.
 * offset - the starting index of the bit field (beginning from 0).
 * bit_count - the number of bits to set in the data.
 * destination - the destination array.
 * destination_length - the total length of the destination array in bytes,
 *      for range checking.
 *
 * Returns true if the bit_count is enough to fully represent the value, and
 *      false if it will not fit.
 */
bool set_bitfield(const uint64_t value, const uint16_t offset,
        const uint16_t bit_count, uint8_t destination[],
        uint16_t destination_length);

/* Public: Return a right aligned bitmask for a uint64_t.
 *
 * bit_count - the number of bits to mask, right aligned.
 */
uint64_t bitmask(const uint8_t bit_count);

/* Private:
 */
uint16_t bits_to_bytes(uint32_t bits);

/* Private: A union to assist swapping between uint64_t and a uint8_t array.
 */
typedef union {
    uint64_t whole;
    uint8_t bytes[sizeof(uint64_t)];
} ArrayOrBytes;

#ifdef __cplusplus
}
#endif

#endif // __BITFIELD_H__
