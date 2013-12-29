#include <bitfield/bitfield.h>
#include <limits.h>
#include <string.h>
#include <stddef.h>

#define NIBBLE_SIZE (CHAR_BIT / 2)

#define PREPARE_FIRST_COPY()                                      \
    do {                                                          \
    if (source_length >= (CHAR_BIT - destination_offset_modulo)) {              \
        *destination     &= reverse_mask[destination_offset_modulo];              \
        source_length -= CHAR_BIT - destination_offset_modulo;                  \
    } else {                                                      \
        *destination     &= reverse_mask[destination_offset_modulo]               \
              | reverse_mask_xor[destination_offset_modulo + source_length + 1];\
         c       &= reverse_mask[destination_offset_modulo + source_length    ];\
        source_length = 0;                                              \
    } } while (0)

/**
 * Find the ending bit of a bitfield within the final byte.
 *
 * Returns: a bit position from 0 to 7.
 */
static uint8_t findEndBit(const uint16_t startBit, const uint16_t numBits) {
    int endBit = numBits % CHAR_BIT;
    return endBit == 0 ? CHAR_BIT : endBit;
}


// TODO can probably remove this
static int byteForBit(const uint16_t startBit) {
    return startBit / CHAR_BIT;
}

/* Thanks to
 * http://stackoverflow.com/questions/3534535/whats-a-time-efficient-algorithm-to-copy-unaligned-bit-arrays
 */
static void bitarray_copy(const uint8_t* source_origin, int source_offset,
        int source_length, uint8_t* destination_origin, int destination_offset) {
    static const uint8_t reverse_mask[] =
        { 0x55, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff };
    static const uint8_t reverse_mask_xor[] =
        { 0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01, 0x00 };

    if(source_length < 1) {
        return;
    }

    const uint8_t* source = source_origin + byteForBit(source_offset);
    uint8_t* destination = destination_origin + byteForBit(destination_offset);
    int source_offset_modulo = source_offset % CHAR_BIT;
    int destination_offset_modulo = destination_offset % CHAR_BIT;

    if(source_offset_modulo == destination_offset_modulo) {
        if(source_offset_modulo > 0) {
            uint8_t c = reverse_mask_xor[destination_offset_modulo] & *source++;
            PREPARE_FIRST_COPY();
            *destination++ |= c;
        }

        int byte_len = source_length / CHAR_BIT;
        int source_length_modulo = source_length % CHAR_BIT;

        if(byte_len > 0) {
            memcpy(destination, source, byte_len);
            source += byte_len;
            destination += byte_len;
        }

        if(source_length_modulo > 0) {
            *destination &= reverse_mask_xor[source_length_modulo];
            *destination |= reverse_mask[source_length_modulo] & *source;
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
        int byte_len = source_length / CHAR_BIT;
        while(--byte_len >= 0) {
            c = *source++ << bit_diff_left_shift;
            c |= *source >> bit_diff_right_shift;
            *destination++ = c;
        }

        /*
         * End: copy the remaing bits;
         */
        int source_length_modulo = source_length % CHAR_BIT;
        if(source_length_modulo > 0) {
            c = *source++ << bit_diff_left_shift;
            c |= *source >> bit_diff_right_shift;
            c &= reverse_mask[source_length_modulo];

            *destination &= reverse_mask_xor[source_length_modulo];
            *destination |= c;
        }
    }
}

uint64_t bitmask(const uint8_t numBits) {
    return (((uint64_t)0x1) << numBits) - 1;
}

uint64_t getBitField(uint64_t data, const uint16_t startBit,
        const uint16_t numBits, bool bigEndian) {
    uint8_t result[8] = {0};
    if(!bigEndian) {
        data = __builtin_bswap64(data);
    }
    getBits(startBit, numBits, (const uint8_t*)&data,
            CHAR_BIT * sizeof(uint64_t),
            bigEndian ? ENDIANNESS_BIG_ENDIAN : ENDIANNESS_LITTLE_ENDIAN,
            result);
    // TODO the result has already been shifted to be aligned right, so if we
    // try and bswap here it's going to be screwed up unless it was byte aligned
    uint64_t int_result = 0;
    // TODO should the API return the byte length of data in the result array?
    // i think yes.
    uint8_t byte_count = numBits / CHAR_BIT;
    if(numBits % CHAR_BIT != 0) {
        ++byte_count;
    }

    // TODO wow, can't believe this works, but something is clearly wrong with
    // the API!
    for(int i = 0; i < byte_count; i++) {
        int_result |= result[byte_count - i - 1] << (CHAR_BIT * i);
    }
    return int_result;
}

/**
 * TODO it would be nice to have a warning if you call with this a value that
 * won't fit in the number of bits you've specified it should use.
 */
void setBitField(uint64_t* data, uint64_t value, int startBit, int numBits) {
    int shiftDistance = 64 - startBit - numBits;
    value <<= shiftDistance;
    *data &= ~(bitmask(numBits) << shiftDistance);
    *data |= value;
}

uint8_t nthByte(uint64_t source, int byteNum) {
    return (source >> (64 - ((byteNum + 1) * CHAR_BIT))) & 0xFF;
}

uint8_t getNibble(const uint8_t nibble_index, const uint8_t data[],
        const uint8_t length, Endianness endianness) {
    uint8_t byte_index = nibble_index / 2;
    uint8_t result;
    if(byte_index < length) {
        result = data[byte_index];
        if(nibble_index % 2 == 0) {
            result >>= NIBBLE_SIZE;
        }
    }
    result &= bitmask(NIBBLE_SIZE);
    return result;
}

// TODO getBytes, return status and store in output parameter
uint8_t getByte(const uint8_t byte_index, const uint8_t data[],
        const uint8_t length, Endianness endianness) {
    if(byte_index < length) {
        return data[byte_index];
    }
    return 0;
}

void getBits(const uint16_t start_index, const uint16_t field_size,
        const uint8_t data[], const uint8_t length, Endianness endianness,
        uint8_t* result) {
    bitarray_copy(data, start_index, field_size, result,
            CHAR_BIT - findEndBit(start_index, field_size));
}
