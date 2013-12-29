#ifndef __BITFIELD_H__
#define __BITFIELD_H__

#include <stdint.h>
#include <stdbool.h>

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
uint8_t getNibble(const uint8_t source[], const uint8_t source_length,
                const uint8_t nibble_index);

/* Public: Return a single byte from the byte array, with range checking.
 *
 * source - the source byte array.
 * source_length - the total length of the source array.
 * byte_index - the index of the byte to retreive. The leftmost byte is index 0.
 *
 * Returns the retreived byte.
 */
uint8_t getByte(const uint8_t source[], const uint8_t source_length,
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
 *      copyBits(source, sizeof(source), 8, 8, destination,
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
bool copyBits(const uint8_t* source_origin, const uint16_t source_length,
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
 *      copyBitsRightAligned(source, sizeof(source), 8, 8, destination,
 *              sizeof(destination));
 *      // destination[0] == 0x0
 *      // destination[1] == 0x0
 *      // destination[2] == 0x0
 *      // destination[3] == 0x22
 *
 *      int value = (int)destination;
 *      // value == 0x22 == 32
 *
 * The arguments are the same as copyBits, but without the destination_offset
 * option - that's set automatically to right align the result.
 *
 * Returns true if the copy was successful and false if the range exceeded the
 * size of the source or destination, or if the range size negative or 0.
 */
bool copyBitsRightAligned(const uint8_t source[], const uint16_t source_length,
                const uint16_t offset, const uint16_t bit_count,
                uint8_t* destination, const uint16_t destination_length);

// TODO using uint64_t everywhere for CAN message payload is kind of cute, but
// in actuality a CAN message may have a smaller payload, and it makes all of
// these functions not applicable to other data sizes. It's also fairly
// inefficient on 32-bit platforms. how much work is it to switch vi-firmware
// to using uint8_t*?

/* Public: Reads a subset of bits from a byte array.
 *
 * data - the bytes in question.
 * startPos - the starting index of the bit field (beginning from 0).
 * numBits - the width of the bit field to extract.
 * bigEndian - if the data passed in is little endian, set this to false and it
 *      will be flipped before grabbing the bit field.
 *
 * Bit fields are positioned according to big-endian bit layout, but inside the
 * bit field, values are represented as little-endian. Therefore, to get the bit
 * field, we swap the overall byte order if bigEndian == false and
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
 *  uint64_t value = getBitField(data, 2, 4);
 *
 * Returns the value of the requested bit field.
 */
uint64_t getBitField(uint64_t data, const uint16_t startPos,
        const uint16_t numBits, bool bigEndian);

/* Public: Set the bit field in the given data array to the new value.
 *
 * data - a byte array with size at least startPos + numBits.
 * value - the value to set in the bit field.
 * startPos - the starting index of the bit field (beginning from 0).
 */
void setBitField(uint64_t* data, uint64_t value, const uint16_t startPos,
        const uint16_t numBits);

/* Public: Retreive the nth byte out of 8 bytes in a uint64_t.
 *
 * source - the source data to retreive the byte from.
 * byteNum - the index of the byte, starting at 0 and assuming big-endian order.
 *
 * Returns the requested byte from the source bytes.
 */
uint8_t nthByte(const uint64_t source, const uint16_t byteNum);

#ifdef __cplusplus
}
#endif

#endif // __BITFIELD_H__
