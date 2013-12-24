#ifndef __READ_H__
#define __READ_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Public: Parse a CAN signal from a message and apply required transformation.
 *
 * signal - The details of the signal to decode and forward.
 * data   - The raw bytes of the CAN message that contains the signal, assumed
 *      to be in big-endian byte order from CAN.
 *
 * Returns the final, transformed value of the signal.
 */
float parseFloat(uint64_t data, uint8_t bitPosition, uint8_t bitSize,
        float factor, float offset);

bool parseBoolean(uint64_t data, uint8_t bitPosition, uint8_t bitSize,
        float factor, float offset);

#ifdef __cplusplus
}
#endif

#endif // __READ_H__
