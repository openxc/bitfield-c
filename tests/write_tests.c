#include <canutil/write.h>
#include <check.h>
#include <stdint.h>

START_TEST (test_encode_can_signal)
{
    uint64_t value = bitfield_encode_float(0, 1, 3, 1, 0);
    ck_assert_int_eq(value, 0);

    value = bitfield_encode_float(1, 1, 3, 1, 0);
    ck_assert_int_eq(value, 0x1000000000000000LLU);
}
END_TEST

START_TEST (test_encode_can_signal_rounding_precision)
{
    uint64_t value = bitfield_encode_float(50, 2, 19, 0.001, 0);
    ck_assert_int_eq(value, 0x061a800000000000LLU);
}
END_TEST

START_TEST (test_encode_bool)
{
    uint64_t value = bitfield_encode_bool(true, 1, 3);
    ck_assert_int_eq(value, 0x1000000000000000LLU);
    value = bitfield_encode_bool(false, 1, 3);
    ck_assert_int_eq(value, 0x0000000000000000LLU);
}
END_TEST
// TODO test encode bool

Suite* canwriteSuite(void) {
    Suite* s = suite_create("write");
    TCase *tc_core = tcase_create("core");
    tcase_add_checked_fixture(tc_core, NULL, NULL);
    tcase_add_test(tc_core, test_encode_can_signal);
    tcase_add_test(tc_core, test_encode_bool);
    tcase_add_test(tc_core, test_encode_can_signal_rounding_precision);
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
