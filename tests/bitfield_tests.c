#include <check.h>
#include <stdint.h>
#include <bitfield/bitfield.h>

START_TEST (test_large_bitmask)
{
    // yeah, this isn't a public method but I wanted to unit test it to track
    // down a bug
    extern uint64_t bitmask(int numBits);
    uint64_t result = bitmask(32);
    fail_if(result != 0xffffffff);
}
END_TEST

START_TEST (test_one_bit_not_swapped)
{
    uint64_t data = 0x80;
    uint64_t result = getBitField(data, 0, 1, false);
    fail_if(result == 1);
}
END_TEST

START_TEST (test_one_bit)
{
    uint64_t data = 0x8000000000000000;
    uint64_t result = getBitField(data, 0, 1, false);
    fail_unless(result == 0x1,
            "First bit in 0x%llx was 0x%llx instead of 0x1", data, result);
}
END_TEST

START_TEST (test_32_bit_parse)
{
    uint64_t data = 0x0402574d555a0401;
    uint64_t result = getBitField(data, 16, 32, false);
    uint64_t expectedValue = 0x574d555a;
    fail_unless(result == expectedValue,
            "Field retrieved in 0x%llx was 0x%llx instead of 0x%llx", data,
            result, expectedValue);
}
END_TEST

START_TEST (test_16_bit_parse)
{
    uint64_t data = 0xF34DFCFF00000000;
    uint64_t result = getBitField(data, 16, 16, false);
    uint64_t expectedValue = 0xFCFF;
    fail_unless(result == expectedValue,
            "Field retrieved in 0x%llx was 0x%llx instead of 0x%llx", data,
            result, expectedValue);
}
END_TEST

START_TEST (test_one_byte)
{
    uint64_t data = 0xFA00000000000000;
    uint64_t result = getBitField(data, 0, 4, false);
    fail_unless(result == 0xF,
            "First 4 bits in 0x%llx was 0x%llx instead of 0xF", data, result);
    result = getBitField(data, 4, 4, false);
    fail_unless(result == 0xA,
            "First 4 bits in 0x%llx was 0x%llx instead of 0xA", data, result);
    result = getBitField(data, 0, 8, false);
    fail_unless(result == 0xFA,
            "All bits in 0x%llx were 0x%llx instead of 0x%llx", data, result, data);
}
END_TEST

START_TEST (test_multi_byte)
{
    uint64_t data = 0x12FA000000000000;
    uint64_t result = getBitField(data, 0, 4, false);
    fail_unless(result == 0x1,
            "First 4 bits in 0x%llx was 0x%llx instead of 0xF", (data >> 60) & 0xF,
            result);
    result = getBitField(data, 4, 4, false);
    fail_unless(result == 0x2,
            "Second 4 bits in 0x%llx was 0x%llx instead of 0xA", (data >> 56) & 0xF,
            result);
    result = getBitField(data, 8, 4, false);
    fail_unless(result == 0xF,
            "First 4 bits in 0x%llx was 0x%llx instead of 0x1", (data >> 52) & 0xF,
            result);
    result = getBitField(data, 12, 4, false);
    fail_unless(result == 0xA,
            "Second 4 bits in 0x%llx was 0x%llx instead of 0x2", (data >> 48) % 0xF,
            result);
}
END_TEST

START_TEST (test_get_multi_byte)
{
    uint64_t data = 0x12FA000000000000;
    uint64_t result = getBitField(data, 0, 9, false);
    ck_assert_int_eq(result, 0x25);
}
END_TEST

START_TEST (test_get_off_byte_boundary)
{
    uint64_t data = 0x000012FA00000000;
    uint64_t result = getBitField(data, 12, 8, false);
    ck_assert_int_eq(result, 0x01);
} END_TEST

START_TEST (test_set_field)
{
    uint64_t data = 0;
    setBitField(&data, 1, 0, 1);
    uint64_t result = getBitField(data, 0, 1, false);
    ck_assert_int_eq(result, 0x1);
    data = 0;
    setBitField(&data, 1, 1, 1);
    result = getBitField(data, 1, 1, false);
    ck_assert_int_eq(result, 0x1);

    data = 0;
    setBitField(&data, 0xf, 3, 4);
    result = getBitField(data, 3, 4, false);
    ck_assert_int_eq(result, 0xf);
}
END_TEST

START_TEST (test_set_doesnt_clobber_existing_data)
{
    uint64_t data = 0xFFFC4DF300000000;
    setBitField(&data, 0x4fc8, 16, 16);
    uint64_t result = getBitField(data, 16, 16, false);
    fail_unless(result == 0x4fc8,
            "Field retrieved in 0x%llx was 0x%llx instead of 0x%x", data, result,
            0xc84f);

    data = 0x8000000000000000;
    setBitField(&data, 1, 21, 1);
    fail_unless(data == 0x8000040000000000LLU,
            "Expected combined value 0x8000040000000000 but got 0x%llx%llx",
            data >> 32, data);
}
END_TEST

START_TEST (test_set_off_byte_boundary)
{
    uint64_t data = 0xFFFC4DF300000000;
    setBitField(&data, 0x12, 12, 8);
    uint64_t result = getBitField(data, 12, 12, false);
    ck_assert_int_eq(result,0x12d);
}
END_TEST

START_TEST (test_set_odd_number_of_bits)
{
    uint64_t data = 0xFFFC4DF300000000LLU;
    setBitField(&data, 0x12, 11, 5);
    uint64_t result = getBitField(data, 11, 5, false);
    fail_unless(result == 0x12,
            "Field set in 0x%llx%llx%llx%llx was 0x%llx instead of 0x%llx", data, result,
            0x12);

    data = 0xFFFC4DF300000000LLU;
    setBitField(&data, 0x2, 11, 5);
    result = getBitField(data, 11, 5, false);
    fail_unless(result == 0x2,
            "Field set in 0x%llx%llx%llx%llx was 0x%llx instead of 0x%llx", data, result,
            0x2);
}
END_TEST

START_TEST(test_nth_byte)
{
    uint64_t data = 0x00000000F34DFCFF;
    uint8_t result = nthByte(data, 0);
    uint8_t expected = 0x0;
    ck_assert_int_eq(result, expected);

    result = nthByte(data, 4);
    expected = 0xF3;
    ck_assert_int_eq(result, expected);

    result = nthByte(data, 5);
    expected = 0x4D;
    ck_assert_int_eq(result, expected);

    result = nthByte(data, 6);
    expected = 0xFC;
    ck_assert_int_eq(result, expected);

    result = nthByte(data, 7);
    expected = 0xFF;
    ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST (test_get_byte)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result = getByte(0, data, sizeof(data), ENDIANNESS_BIG_ENDIAN);
    ck_assert_int_eq(result, 0x12);
    result = getByte(3, data, sizeof(data), ENDIANNESS_BIG_ENDIAN);
    ck_assert_int_eq(result, 0x78);
}
END_TEST

START_TEST (test_get_nibble)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result = getNibble(0, data, sizeof(data), ENDIANNESS_BIG_ENDIAN);
    ck_assert_int_eq(result, 0x1);
    result = getNibble(1, data, sizeof(data), ENDIANNESS_BIG_ENDIAN);
    ck_assert_int_eq(result, 0x2);
    result = getNibble(2, data, sizeof(data), ENDIANNESS_BIG_ENDIAN);
    ck_assert_int_eq(result, 0x3);
}
END_TEST

START_TEST (test_get_bits)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result[4];
    getBits(0, 16, data, 36, ENDIANNESS_BIG_ENDIAN, result);
    ck_assert_int_eq(result[0], 0x12);
    ck_assert_int_eq(result[1], 0x34);
}
END_TEST

START_TEST (test_get_uneven_bits)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result[4] = {0};
    getBits(4, 12, data, 36, ENDIANNESS_BIG_ENDIAN, result);
    ck_assert_int_eq(result[0], 0x2);
    ck_assert_int_eq(result[1], 0x34);
}
END_TEST

Suite* bitfieldSuite(void) {
    Suite* s = suite_create("bitfield");
    TCase *tc_core = tcase_create("core");
    tcase_add_test(tc_core, test_large_bitmask);
    tcase_add_test(tc_core, test_one_bit);
    tcase_add_test(tc_core, test_one_bit_not_swapped);
    tcase_add_test(tc_core, test_one_byte);
    tcase_add_test(tc_core, test_16_bit_parse);
    tcase_add_test(tc_core, test_32_bit_parse);
    tcase_add_test(tc_core, test_multi_byte);
    tcase_add_test(tc_core, test_get_multi_byte);
    tcase_add_test(tc_core, test_get_off_byte_boundary);
    tcase_add_test(tc_core, test_set_field);
    tcase_add_test(tc_core, test_set_doesnt_clobber_existing_data);
    tcase_add_test(tc_core, test_set_off_byte_boundary);
    tcase_add_test(tc_core, test_set_odd_number_of_bits);
    tcase_add_test(tc_core, test_nth_byte);
    tcase_add_test(tc_core, test_get_byte);
    tcase_add_test(tc_core, test_get_nibble);
    tcase_add_test(tc_core, test_get_bits);
    tcase_add_test(tc_core, test_get_uneven_bits);
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
