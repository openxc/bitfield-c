#include <canutil/write.h>
#include <check.h>
#include <stdint.h>

START_TEST (test_eightbyte_encode_float_precision)
{
    uint64_t value = eightbyte_encode_float(50, 2, 19, 0.001, 0);
    ck_assert_int_eq(value, 0x061a800000000000LLU);
}
END_TEST

START_TEST (test_eightbyte_encode_float)
{
    uint64_t value = eightbyte_encode_float(0, 1, 3, 1, 0);
    ck_assert_int_eq(value, 0);

    value = eightbyte_encode_float(1, 1, 3, 1, 0);
    ck_assert_int_eq(value, 0x1000000000000000LLU);
}
END_TEST

START_TEST (test_eightbyte_encode_bool)
{
    uint64_t value = eightbyte_encode_bool(true, 1, 3);
    ck_assert_int_eq(value, 0x1000000000000000LLU);
    value = eightbyte_encode_bool(false, 1, 3);
    ck_assert_int_eq(value, 0x0000000000000000LLU);
}
END_TEST

START_TEST (test_bitfield_encode_float)
{
    uint8_t data[8] = {0};
    bitfield_encode_float(0, 1, 3, 1, 0, data, sizeof(data));
    ck_assert_int_eq(data[0], 0);
    ck_assert_int_eq(data[1], 0);
    ck_assert_int_eq(data[2], 0);
    ck_assert_int_eq(data[3], 0);
    ck_assert_int_eq(data[4], 0);
    ck_assert_int_eq(data[5], 0);
    ck_assert_int_eq(data[6], 0);
    ck_assert_int_eq(data[7], 0);

    bitfield_encode_float(1, 1, 3, 1, 0, data, sizeof(data));
    ck_assert_int_eq(data[0], 0x10);
    ck_assert_int_eq(data[1], 0);
    ck_assert_int_eq(data[2], 0);
    ck_assert_int_eq(data[3], 0);
    ck_assert_int_eq(data[4], 0);
    ck_assert_int_eq(data[5], 0);
    ck_assert_int_eq(data[6], 0);
    ck_assert_int_eq(data[7], 0);
}
END_TEST

START_TEST (test_bitfield_encode_bool)
{
    uint8_t data[8] = {0};
    bitfield_encode_bool(true, 1, 3, data, sizeof(data));
    ck_assert_int_eq(data[0], 0x10);
    ck_assert_int_eq(data[1], 0);
    ck_assert_int_eq(data[2], 0);
    ck_assert_int_eq(data[3], 0);
    ck_assert_int_eq(data[4], 0);
    ck_assert_int_eq(data[5], 0);
    ck_assert_int_eq(data[6], 0);
    ck_assert_int_eq(data[7], 0);

    bitfield_encode_bool(false, 1, 3, data, sizeof(data));
    ck_assert_int_eq(data[0], 0);
    ck_assert_int_eq(data[1], 0);
    ck_assert_int_eq(data[2], 0);
    ck_assert_int_eq(data[3], 0);
    ck_assert_int_eq(data[4], 0);
    ck_assert_int_eq(data[5], 0);
    ck_assert_int_eq(data[6], 0);
    ck_assert_int_eq(data[7], 0);
}
END_TEST

Suite* canwriteSuite(void) {
    Suite* s = suite_create("write");
    TCase *tc_core = tcase_create("core");
    tcase_add_checked_fixture(tc_core, NULL, NULL);
    tcase_add_test(tc_core, test_eightbyte_encode_float);
    tcase_add_test(tc_core, test_eightbyte_encode_bool);
    tcase_add_test(tc_core, test_eightbyte_encode_float_precision);
    tcase_add_test(tc_core, test_bitfield_encode_float);
    tcase_add_test(tc_core, test_bitfield_encode_bool);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int numberFailed;
    Suite* s = canwriteSuite();
    SRunner *sr = srunner_create(s);
    // Don't fork so we can actually use gdb
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    numberFailed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (numberFailed == 0) ? 0 : 1;
}
