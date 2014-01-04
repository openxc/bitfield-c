#ifndef __READ_H__
#define __READ_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Public: Parse a CAN signal from a message and apply required transformation.
 *
 * source - the payload containing the signal.
 * bit_offset - the starting bit for the signal.
 * bit_size - the width of the signal.
 * factor - the transformation factor for the signal value, applied after
 *      pulling out the bit field. Use 1.0 for no factor.
 * offset - the transformation offset for the signal value, applied after
 *      pulling out the bit field. Use 0 for no offset.
 *
 * Returns the decoded and transformed value of the signal.
 */
float eightbyte_parse_float(const uint64_t source, const uint8_t bit_offset,
        const uint8_t bit_size, const float factor, const float offset);

/* Public: Parse a CAN signal from a message storage as a byte array and apply
 * required transformation.
 *
 * source - the payload containing the signal.
 * source_size - the size of the payload in bytes.
 * bit_offset - the starting bit for the signal.
 * bit_size - the width of the signal.
 * factor - the transformation factor for the signal value, applied after
 *      pulling out the bit field. Use 1.0 for no factor.
 * offset - the transformation offset for the signal value, applied after
 *      pulling out the bit field. Use 0 for no offset.
 *
 * Returns the decoded and transformed value of the signal.
 */
float bitfield_parse_float(const uint8_t source[], const uint16_t source_size,
        const uint8_t bit_offset, const uint8_t bit_size, const float factor,
        const float offset);

/* Public: Parse a CAN signal from a message and interpret it as a boolean.
 *
 * source - the payload containing the signal.
 * bit_offset - the starting bit for the signal.
 * bit_size - the width of the signal.
 * factor - the transformation factor for the signal value, applied after
 *      pulling out the bit field. Use 1.0 for no factor.
 * offset - the transformation offset for the signal value, applied after
 *      pulling out the bit field. Use 0 for no offset.
 *
 * Returns false if the value was 0, otherwise true.
 */
bool eightbyte_parse_bool(uint64_t source, uint8_t bit_offset, uint8_t bit_size,
        float factor, float offset);

/* Public: Parse a CAN signal from a message storage as a byte array and
 * interpret it as a boolean.
 *
 * source - the payload containing the signal.
 * source_size - the size of the payload in bytes.
 * bit_offset - the starting bit for the signal.
 * bit_size - the width of the signal.
 * factor - the transformation factor for the signal value, applied after
 *      pulling out the bit field. Use 1.0 for no factor.
 * offset - the transformation offset for the signal value, applied after
 *      pulling out the bit field. Use 0 for no offset.
 *
 * Returns false if the value was 0, otherwise true.
 */
bool bitfield_parse_bool(const uint8_t source[], const uint16_t source_size,
        const uint8_t bit_offset, const uint8_t bit_size, const float factor,
        const float offset);

#ifdef __cplusplus
}
#endif

#endif // __READ_H__
