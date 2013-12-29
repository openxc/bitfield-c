#include <check.h>
#include <stdint.h>
#include <bitfield/bitfield.h>

START_TEST (test_get_byte)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result = getByte(data, sizeof(data), 0);
    ck_assert_int_eq(result, 0x12);
    result = getByte(data, sizeof(data), 3);
    ck_assert_int_eq(result, 0x78);
}
END_TEST

START_TEST (test_get_nibble)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result = getNibble(data, sizeof(data), 0);
    ck_assert_int_eq(result, 0x1);
    result = getNibble(data, sizeof(data), 1);
    ck_assert_int_eq(result, 0x2);
    result = getNibble(data, sizeof(data), 2);
    ck_assert_int_eq(result, 0x3);
}
END_TEST

START_TEST (test_get_bits_out_of_range)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result[4];
    fail_if(copyBitsRightAligned(data, 4, 25, 16, result, 4));
}
END_TEST

START_TEST (test_get_bits)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result[4] = {0};
    fail_unless(copyBitsRightAligned(data, 4, 0, 16, result, 4));
    ck_assert_int_eq(result[0], 0x12);
    ck_assert_int_eq(result[1], 0x34);
}
END_TEST

START_TEST (test_get_uneven_bits)
{
    uint8_t data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t result[4] = {0};
    fail_unless(copyBitsRightAligned(data, 4, 4, 12, result, 4));
    ck_assert_int_eq(result[0], 0x2);
    ck_assert_int_eq(result[1], 0x34);
}
END_TEST

Suite* bitfieldSuite(void) {
    Suite* s = suite_create("bitfield");
    TCase *tc_core = tcase_create("core");
    tcase_add_test(tc_core, test_get_byte);
    tcase_add_test(tc_core, test_get_nibble);
    tcase_add_test(tc_core, test_get_bits);
    tcase_add_test(tc_core, test_get_bits_out_of_range);
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
