#include <check.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <bitfield/bitfield.h>

START_TEST (test_get_byte)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result = get_byte(data, sizeof(data), 0);
    ck_assert_int_eq(result, 0x12);
    result = get_byte(data, sizeof(data), 3);
    ck_assert_int_eq(result, 0x78);
}
END_TEST

START_TEST (test_set_nibble)
{
    uint8_t data[4] = {0};
    fail_unless(set_nibble(0, 0x1, data, sizeof(data)));
    fail_unless(set_nibble(1, 0x2, data, sizeof(data)));
    fail_unless(set_nibble(2, 0x3, data, sizeof(data)));
    fail_unless(set_nibble(3, 0x4, data, sizeof(data)));
    fail_unless(set_nibble(4, 0x5, data, sizeof(data)));
    ck_assert_int_eq(data[0], 0x12);
    ck_assert_int_eq(data[1], 0x34);
    ck_assert_int_eq(data[2], 0x50);
}
END_TEST

START_TEST (test_set_bitfield)
{
    uint8_t data[4] = {0};
    fail_unless(set_bitfield(0x12, 0, 8, data, sizeof(data)));
    fail_unless(set_bitfield(bitmask(3), 10, 3, data, sizeof(data)));
    ck_assert_int_eq(data[0], 0x12);
    ck_assert_int_eq(data[1], 0x38);
    fail_unless(set_bitfield(0x1, 23, 1, data, sizeof(data)));
    ck_assert_int_eq(data[2], 0x1);
    fail_unless(set_bitfield(0x1, 21, 2, data, sizeof(data)));
    ck_assert_int_eq(data[2], 0x3);
}
END_TEST

START_TEST (test_set_bitfield_doesnt_fit)
{
    uint8_t data[4] = {0};
    fail_if(set_bitfield(0xffff, 0, 8, data, sizeof(data)));
    ck_assert_int_eq(data[0], 0);
    ck_assert_int_eq(data[1], 0);
    ck_assert_int_eq(data[2], 0);
    ck_assert_int_eq(data[3], 0);
}
END_TEST

START_TEST (test_get_nibble)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result = get_nibble(data, sizeof(data), 0);
    ck_assert_int_eq(result, 0x1);
    result = get_nibble(data, sizeof(data), 1);
    ck_assert_int_eq(result, 0x2);
    result = get_nibble(data, sizeof(data), 2);
    ck_assert_int_eq(result, 0x3);
}
END_TEST

START_TEST (test_get_bits_out_of_range)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result[4];
    fail_if(copy_bits_right_aligned(data, sizeof(data), 25, 16, result,
                sizeof(result)));
}
END_TEST

START_TEST (test_get_bits)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result[4] = {0};
    fail_unless(copy_bits_right_aligned(data, sizeof(data), 0, 16, result,
                sizeof(result)));
    ck_assert_int_eq(result[2], 0x12);
    ck_assert_int_eq(result[3], 0x34);
}
END_TEST

START_TEST (test_copy_bytes)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result[4] = {0};
    fail_unless(copy_bytes_right_aligned(data, sizeof(data), 1, 3, result,
                sizeof(result)));
    ck_assert_int_eq(result[1], 0x34);
    ck_assert_int_eq(result[2], 0x56);
    ck_assert_int_eq(result[3], 0x78);
}
END_TEST

START_TEST (test_get_uneven_bits)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result[4] = {0};
    fail_unless(copy_bits_right_aligned(data, sizeof(data), 4, 12, result,
                sizeof(result)));
    ck_assert_int_eq(result[2], 0x2);
    ck_assert_int_eq(result[3], 0x34);
}
END_TEST

START_TEST (test_get_bitfiled)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint64_t result = get_bitfield(data, sizeof(data), 8, 16);
    ck_assert_int_eq(result, 0x3456);
}
END_TEST

START_TEST (test_copy_bits_initial_source_boundary)
{
    uint8_t source[1] = {0x01};
    uint8_t destination[1] = {0x55};

    fail_unless(copy_bits(source, sizeof(source), 7, 1, destination,
                sizeof(destination), 0));
    ck_assert_int_eq(destination[0], 0xd5);
}
END_TEST

START_TEST (test_copy_bits_final_source_boundary)
{
    uint8_t source[1] = {0xa7};
    uint8_t destination[2] = {0x80, 0x55};

    fail_unless(copy_bits(source, sizeof(source), 0, 8, destination,
                sizeof(destination), 1));
    ck_assert_int_eq(destination[0], 0xd3);
    ck_assert_int_eq(destination[1], 0xd5);
}
END_TEST

static void reference_copy_bits(const uint8_t* source, uint16_t source_offset,
        uint16_t bit_count, uint8_t* destination, uint16_t destination_offset) {
    for(uint16_t bit = 0; bit < bit_count; ++bit) {
        uint16_t source_bit = source_offset + bit;
        uint16_t destination_bit = destination_offset + bit;
        uint8_t mask = 1u << (CHAR_BIT - 1 - destination_bit % CHAR_BIT);
        if(source[source_bit / CHAR_BIT] &
                (1u << (CHAR_BIT - 1 - source_bit % CHAR_BIT))) {
            destination[destination_bit / CHAR_BIT] |= mask;
        } else {
            destination[destination_bit / CHAR_BIT] &= ~mask;
        }
    }
}

START_TEST (test_copy_bits_against_reference)
{
    const uint8_t patterns[][4] = {
        {0x96, 0x3d, 0xe2, 0x4b},
        {0x00, 0x00, 0x00, 0x00},
        {0xff, 0xff, 0xff, 0xff}
    };

    for(size_t pattern = 0; pattern < sizeof(patterns) / sizeof(patterns[0]);
            ++pattern) {
        for(uint16_t source_length = 1; source_length <= 4; ++source_length) {
            /* Exact-size allocations expose unnecessary reads under ASan. */
            uint8_t* source = malloc(source_length);
            fail_unless(source != NULL);
            memcpy(source, patterns[pattern], source_length);
            for(uint16_t destination_length = 1; destination_length <= 4;
                    ++destination_length) {
                uint8_t* destination = malloc(destination_length);
                uint8_t expected[4];
                fail_unless(destination != NULL);
                for(uint16_t source_offset = 0;
                        source_offset < source_length * CHAR_BIT;
                        ++source_offset) {
                    for(uint16_t destination_offset = 0;
                            destination_offset < destination_length * CHAR_BIT;
                            ++destination_offset) {
                        uint16_t source_remaining =
                                source_length * CHAR_BIT - source_offset;
                        uint16_t destination_remaining =
                                destination_length * CHAR_BIT - destination_offset;
                        uint16_t maximum = source_remaining < destination_remaining ?
                                source_remaining : destination_remaining;
                        for(uint16_t bit_count = 1; bit_count <= maximum;
                                ++bit_count) {
                            memset(destination, 0xa5, destination_length);
                            memset(expected, 0xa5, destination_length);
                            reference_copy_bits(source, source_offset, bit_count,
                                    expected, destination_offset);
                            fail_unless(copy_bits(source, source_length,
                                        source_offset, bit_count, destination,
                                        destination_length, destination_offset));
                            for(uint16_t byte = 0; byte < destination_length; ++byte) {
                                ck_assert_int_eq(destination[byte], expected[byte]);
                            }
                        }
                    }
                }
                free(destination);
            }
            free(source);
        }
    }
}
END_TEST

START_TEST (test_copy_bits_invalid_ranges)
{
    uint8_t source[2] = {0x96, 0x3d};
    const uint16_t ranges[][3] = {
        {0, 0, 0},
        {2 * CHAR_BIT, 1, 0},
        {0, 2 * CHAR_BIT + 1, 0},
        {0, 1, CHAR_BIT},
        {0, CHAR_BIT + 1, 0}
    };

    for(size_t range = 0; range < sizeof(ranges) / sizeof(ranges[0]); ++range) {
        uint8_t destination[1] = {0xa5};
        fail_if(copy_bits(source, sizeof(source), ranges[range][0],
                    ranges[range][1], destination, sizeof(destination),
                    ranges[range][2]));
        ck_assert_int_eq(destination[0], 0xa5);
    }
}
END_TEST

Suite* bitfieldSuite(void) {
    Suite* s = suite_create("bitfield");
    TCase *tc_core = tcase_create("core");
    tcase_add_test(tc_core, test_get_byte);
    tcase_add_test(tc_core, test_get_nibble);
    tcase_add_test(tc_core, test_set_nibble);
    tcase_add_test(tc_core, test_set_bitfield);
    tcase_add_test(tc_core, test_set_bitfield_doesnt_fit);
    tcase_add_test(tc_core, test_get_bits);
    tcase_add_test(tc_core, test_copy_bytes);
    tcase_add_test(tc_core, test_get_bits_out_of_range);
    tcase_add_test(tc_core, test_get_uneven_bits);
    tcase_add_test(tc_core, test_get_bitfiled);
    tcase_add_test(tc_core, test_copy_bits_initial_source_boundary);
    tcase_add_test(tc_core, test_copy_bits_final_source_boundary);
    tcase_add_test(tc_core, test_copy_bits_against_reference);
    tcase_add_test(tc_core, test_copy_bits_invalid_ranges);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int numberFailed;
    Suite* s = bitfieldSuite();
    SRunner *sr = srunner_create(s);
    // Don't fork so we can actually use gdb
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    numberFailed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (numberFailed == 0) ? 0 : 1;
}
