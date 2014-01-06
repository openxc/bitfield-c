#include <check.h>
#include <stdint.h>
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
