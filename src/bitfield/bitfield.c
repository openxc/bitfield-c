#include <bitfield/bitfield.h>
#include <limits.h>
#include <string.h>
#include <stddef.h>

#ifndef _MSC_VER
#include <sys/param.h>

static inline uint64_t bswap64(uint64_t v)
{
    return __builtin_bswap64(v);
}

static inline bool is_little_endian(void)
{
    return BYTE_ORDER == LITTLE_ENDIAN;
}

#else

static inline uint64_t bswap64(uint64_t v)
{
    return  ((v << 56) & 0xff00000000000000ULL) |
            ((v << 40) & 0x00ff000000000000ULL) |
            ((v << 24) & 0x0000ff0000000000ULL) |
            ((v << 8)  & 0x000000ff00000000ULL) |
            ((v >> 8)  & 0x00000000ff000000ULL) |
            ((v >> 24) & 0x0000000000ff0000ULL) |
            ((v >> 40) & 0x000000000000ff00ULL) |
            ((v >> 56) & 0x00000000000000ffULL);
}

static inline bool is_little_endian(void)
{
    union EndianTest
    {
        uint32_t word;
        uint8_t bytes[4];
    };

    static const union EndianTest ENDIAN_TEST =
    {
        .word = 1
    };

    return ENDIAN_TEST.bytes[0] == 1;
}
#endif

uint64_t bitmask ( const uint8_t bit_count )
{
    if ( bit_count >= 64 )
    {
        return 0xFFFFFFFFFFFFFFFF;
    }
    else
    {
        return ( ( ( uint64_t ) 0x1 ) << bit_count ) - 1;
    }
}

uint8_t get_nibble(const uint8_t source[], const uint8_t source_length,
                const uint8_t nibble_index) {
    uint8_t byte_index = nibble_index / 2;
    uint8_t result = get_byte(source, source_length, byte_index);
    if(nibble_index % 2 == 0) {
        result >>= NIBBLE_SIZE;
    }
    result &= bitmask(NIBBLE_SIZE);
    return result;
}

uint8_t get_byte(const uint8_t source[], const uint8_t source_length,
        const uint8_t byte_index) {
    if(byte_index < source_length) {
        return source[byte_index];
    }
    return 0;
}

uint64_t get_bitfield(const uint8_t source[], const uint16_t source_length,
                const uint16_t offset, const uint16_t bit_count) {
    if(bit_count > 64 || bit_count < 1) {
        // TODO error reporting?
        return 0;
    }

    ArrayOrBytes combined;
    memset(combined.bytes, 0, sizeof(combined.bytes));
    if(copy_bits_right_aligned(source, source_length, offset, bit_count,
            combined.bytes, sizeof(combined.bytes))) {
        if(is_little_endian()) {
            combined.whole = bswap64(combined.whole);
        }
    } else {
        // debug("couldn't copy enough bits from source")
    }
    return combined.whole;
}

bool set_nibble(const uint16_t nibble_index, const uint8_t value,
        uint8_t* destination, const uint16_t destination_length) {
    return copy_bits(&value, CHAR_BIT, NIBBLE_SIZE, NIBBLE_SIZE, destination,
            destination_length, nibble_index * NIBBLE_SIZE);
}

bool set_bitfield(const uint64_t value, const uint16_t offset,
        const uint16_t bit_count, uint8_t destination[],
        uint16_t destination_length) {
    if(value > bitmask(bit_count)) {
        return false;
    }

    ArrayOrBytes combined = {
        .whole=value
    };

    if (is_little_endian()) {
        combined.whole = bswap64(combined.whole);
    }

    return copy_bits(combined.bytes, sizeof(combined.bytes),
            sizeof(combined.bytes) * CHAR_BIT - bit_count, bit_count,
            destination, destination_length, offset);
}
