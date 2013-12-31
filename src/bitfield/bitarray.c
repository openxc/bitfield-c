#include <bitfield/bitfield.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>

#define PREPARE_FIRST_COPY()                                      \
    do {                                                          \
    if (bit_count >= (CHAR_BIT - destination_offset_modulo)) {              \
        *destination &= reverse_mask[destination_offset_modulo];              \
        bit_count -= CHAR_BIT - destination_offset_modulo;                  \
    } else {                                                      \
        *destination &= reverse_mask[destination_offset_modulo]               \
              | reverse_mask_xor[destination_offset_modulo + bit_count + 1];\
         c &= reverse_mask[destination_offset_modulo + bit_count    ];\
        bit_count = 0;                                              \
    } } while (0)

static const uint8_t reverse_mask[] =
    { 0x55, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff };
static const uint8_t reverse_mask_xor[] =
    { 0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01, 0x00 };

bool copy_bits(const uint8_t* source_origin, const uint16_t source_length,
        const uint16_t source_offset, uint16_t bit_count,
        uint8_t* destination_origin, const uint16_t destination_length,
        const uint16_t destination_offset) {
    if(bit_count < 1) {
        return false;
    }

    if(source_offset + bit_count > source_length * CHAR_BIT ||
            destination_offset + bit_count > destination_length * CHAR_BIT ) {
        return false;
    }

    const uint8_t* source = source_origin + (source_offset / CHAR_BIT);
    uint8_t* destination = destination_origin + (destination_offset / CHAR_BIT);
    int source_offset_modulo = source_offset % CHAR_BIT;
    int destination_offset_modulo = destination_offset % CHAR_BIT;

    if(source_offset_modulo == destination_offset_modulo) {
        if(source_offset_modulo > 0) {
            uint8_t c = reverse_mask_xor[destination_offset_modulo] & *source++;
            PREPARE_FIRST_COPY();
            *destination++ |= c;
        }

        int byte_len = bit_count / CHAR_BIT;
        int bit_count_modulo = bit_count % CHAR_BIT;

        if(byte_len > 0) {
            memcpy(destination, source, byte_len);
            source += byte_len;
            destination += byte_len;
        }

        if(bit_count_modulo > 0) {
            *destination &= reverse_mask_xor[bit_count_modulo];
            *destination |= reverse_mask[bit_count_modulo] & *source;
        }
    } else {
        int bit_diff_left_shift;
        int bit_diff_right_shift;
        uint8_t c;
        /*
         * Begin: Line things up on destination.
         */
        if(source_offset_modulo > destination_offset_modulo) {
            bit_diff_left_shift = source_offset_modulo - destination_offset_modulo;
            bit_diff_right_shift = CHAR_BIT - bit_diff_left_shift;

            c = *source++ << bit_diff_left_shift;
            c |= *source >> bit_diff_right_shift;
            c &= reverse_mask_xor[destination_offset_modulo];
        } else {
            bit_diff_right_shift = destination_offset_modulo - source_offset_modulo;
            bit_diff_left_shift = CHAR_BIT - bit_diff_right_shift;

            c = *source >> bit_diff_right_shift &
                    reverse_mask_xor[destination_offset_modulo];
        }
        PREPARE_FIRST_COPY();
        *destination++ |= c;

        /*
         * Middle: copy with only shifting the source.
         */
        int byte_len = bit_count / CHAR_BIT;
        while(--byte_len >= 0) {
            c = *source++ << bit_diff_left_shift;
            c |= *source >> bit_diff_right_shift;
            *destination++ = c;
        }

        /*
         * End: copy the remaing bits;
         */
        int bit_count_modulo = bit_count % CHAR_BIT;
        if(bit_count_modulo > 0) {
            c = *source++ << bit_diff_left_shift;
            c |= *source >> bit_diff_right_shift;
            c &= reverse_mask[bit_count_modulo];

            *destination &= reverse_mask_xor[bit_count_modulo];
            *destination |= c;
        }
    }
    return true;
}

uint16_t bits_to_bytes(uint32_t bits) {
    uint8_t byte_count = bits / CHAR_BIT;
    if(bits % CHAR_BIT != 0) {
        ++byte_count;
    }
    return byte_count;
}

/**
 * Find the ending bit of a bitfield within the final byte.
 *
 * Returns: a bit position from 0 to 7.
 */
uint8_t find_end_bit(const uint16_t numBits) {
    int endBit = numBits % CHAR_BIT;
    return endBit == 0 ? CHAR_BIT : endBit;
}

bool copy_bits_right_aligned(const uint8_t source[], const uint16_t source_length,
                const uint16_t offset, const uint16_t bit_count,
                uint8_t* destination, const uint16_t destination_length) {
    return copy_bits(source, source_length, offset, bit_count, destination,
            destination_length,
            // provide a proper destination offset so the result is right
            // aligned
            (destination_length - bits_to_bytes(bit_count)) * CHAR_BIT +
                 CHAR_BIT - find_end_bit(bit_count));
}

bool copy_bytes_right_aligned(const uint8_t source[], const uint16_t source_length,
                const uint16_t offset, const uint16_t byte_count,
                uint8_t* destination, const uint16_t destination_length) {
    return copy_bits_right_aligned(source, source_length, offset * CHAR_BIT,
            byte_count * CHAR_BIT, destination, destination_length);
}
