// #include "s21_decimal_test.h"

// START_TEST(div_0) {
//   s21_decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
//   s21_decimal val2 = {{2, 0, 0, 0}};
//   s21_decimal res = {{0}};
//   ck_assert_int_eq(0, s21_div(val1, val2, &res));
// }
// END_TEST

// START_TEST(div_1) {
//   s21_decimal val1 = {{2, 0, 0, 0}};
//   s21_decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
//   s21_decimal res = {{0}};
//   ck_assert_int_eq(0, s21_div(val1, val2, &res));
// }
// END_TEST

// START_TEST(div_2) {
//   s21_decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
//   s21_decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
//   s21_decimal res = {{0}};
//   ck_assert_int_eq(0, s21_div(val1, val2, &res));
// }
// END_TEST

// START_TEST(div_3) {
//   s21_decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
//   s21_decimal val2 = {{0, 0, 0, 0}};
//   s21_decimal res = {{0}};
//   ck_assert_int_eq(3, s21_div(val1, val2, &res));
// }
// END_TEST

// START_TEST(div_4) {
//   s21_decimal val1 = {{2, 0, 0, 0}};
//   s21_decimal val2 = {{0, 0, 0, 0}};
//   s21_decimal res = {{0}};
//   ck_assert_int_eq(3, s21_div(val1, val2, &res));
// }
// END_TEST

// START_TEST(div_5) {
//   s21_decimal value_1 = {{35, 0, 0, 0}};
//   s21_decimal value_2 = {{5, 0, 0, 0}};
//   s21_decimal result = {{0, 0, 0, 0}};
//   s21_decimal check = {{70, 0, 0, 0}};
//   s21_set_scale(&value_1, 1);
//   s21_set_scale(&value_2, 2);
//   int return_value = s21_div(value_1, value_2, &result);
//   ck_assert_int_eq(s21_is_equal(result, check), 1);
//   ck_assert_int_eq(return_value, 0);
// }
// END_TEST

// START_TEST(div_6) {
//   s21_decimal value_1 = {{0x88888888, 0x88888888, 0x88888888, 0}};
//   s21_decimal value_2 = {{0x2, 0, 0, 0}};
//   s21_set_sign(&value_2);
//   s21_decimal result = {{0, 0, 0, 0}};
//   s21_decimal check = {{0x44444444, 0x44444444, 0x44444444, 0}};
//   s21_set_sign(&check);
//   int return_value = s21_div(value_1, value_2, &result);
//   ck_assert_int_eq(s21_is_equal(result, check), 1);
//   ck_assert_int_eq(return_value, 0);
// }
// END_TEST

// START_TEST(div_7) {
//   s21_decimal value_1 = {{10, 0, 0, 0}};
//   s21_set_sign(&value_1);
//   s21_decimal value_2 = {{8, 0, 0, 0}};
//   s21_set_sign(&value_2);
//   s21_decimal result = {{0, 0, 0, 0}};
//   s21_decimal check = {{125, 0, 0, 0}};
//   s21_set_scale(&check, 2);
//   int return_value = s21_div(value_1, value_2, &result);
//   ck_assert_int_eq(s21_is_equal(result, check), 1);
//   ck_assert_int_eq(return_value, 0);
// }
// END_TEST

// START_TEST(div_8) {
//   s21_decimal value_1 = {{15, 0, 0, 0}};
//   s21_set_sign(&value_1);
//   s21_decimal value_2 = {{0, 0, 0, 0}};
//   s21_decimal result = {{0, 0, 0, 0}};
//   s21_decimal check = {{0, 0, 0, 0}};
//   int return_value = s21_div(value_1, value_2, &result);
//   ck_assert_int_eq(s21_is_equal(result, check), 1);
//   ck_assert_int_eq(return_value, 3);
// }
// END_TEST

// START_TEST(div_9) {
//   s21_decimal value_1 = {{10, 0, 0, 0}};
//   s21_decimal value_2 = {{1, 0, 0, 0}};
//   s21_set_scale(&value_2, 2);
//   s21_decimal result = {{0, 0, 0, 0}};
//   s21_decimal check = {{1000u, 0, 0, 0}};
//   int return_value = s21_div(value_1, value_2, &result);
//   ck_assert_int_eq(s21_is_equal(result, check), 1);
//   ck_assert_int_eq(return_value, 0);
// }
// END_TEST

// START_TEST(div_10) {
//   s21_decimal src1, src2;
//   float a = 9403.0e2;
//   int b = 202;
//   float res_our_dec = 0.0;
//   s21_from_float_to_decimal(a, &src1);
//   s21_from_int_to_decimal(b, &src2);
//   float res_origin = 4654.950495049504950495049505;
//   s21_decimal res_od;
//   s21_div(src1, src2, &res_od);
//   s21_from_decimal_to_float(res_od, &res_our_dec);
//   ck_assert_float_eq(res_our_dec, res_origin);
// }
// END_TEST

// START_TEST(div_11) {
//   s21_decimal src1, src2;
//   float a = -9403.0e2;
//   float b = -2020.29;
//   float res_our_dec = 0.0;
//   s21_from_float_to_decimal(a, &src1);
//   s21_from_float_to_decimal(b, &src2);
//   float res_origin = 465.4282306005573457275935633;
//   s21_decimal res_od;
//   s21_div(src1, src2, &res_od);
//   s21_from_decimal_to_float(res_od, &res_our_dec);
//   ck_assert_float_eq(res_our_dec, res_origin);
// }
// END_TEST

// START_TEST(div_12) {
//   s21_decimal src1, src2;
//   float a = -9403.0e20;
//   float b = 2.28e17;
//   float res_our_dec = 0.0;
//   s21_from_float_to_decimal(a, &src1);
//   s21_from_float_to_decimal(b, &src2);
//   float res_origin = a / b;
//   s21_decimal res_od;
//   s21_div(src1, src2, &res_od);
//   s21_from_decimal_to_float(res_od, &res_our_dec);
//   ck_assert_float_eq(res_our_dec, res_origin);
// }
// END_TEST

// START_TEST(div_13) {
//   s21_decimal src1, src2;
//   src1.bits[0] = 0b00010001100000110100010101000111;
//   src1.bits[1] = 0b00000000000000101011011100110010;
//   src1.bits[2] = 0b00000000000000000000000000000000;
//   src1.bits[3] = 0b00000000000001010000000000000000;
//   src2.bits[0] = 0b01111001000001000011011001001011;
//   src2.bits[1] = 0b00000000000000000101010011111111;
//   src2.bits[2] = 0b00000000000000000000000000000000;
//   src2.bits[3] = 0b00000000000001010000000000000000;
//   s21_decimal result_origin;
//   result_origin.bits[0] = 0b01000111110000001001001101101011;
//   result_origin.bits[1] = 0b10111000000101111000101110100000;
//   result_origin.bits[2] = 0b00011010011011010111110000110100;
//   result_origin.bits[3] = 0b00000000000110110000000000000000;
//   s21_decimal res_od;
//   s21_div(src1, src2, &res_od);
//   float a, b;
//   s21_from_decimal_to_float(result_origin, &a);
//   s21_from_decimal_to_float(res_od, &b);
//   ck_assert_float_eq(a, b);
// }
// END_TEST

// Suite *suite_div(void) {
//   Suite *s = suite_create("suite_div");
//   TCase *tc = tcase_create("case_div");

//   tcase_add_test(tc, div_0);
//   tcase_add_test(tc, div_1);
//   tcase_add_test(tc, div_2);
//   tcase_add_test(tc, div_3);
//   tcase_add_test(tc, div_4);
//   tcase_add_test(tc, div_5);
//   tcase_add_test(tc, div_6);
//   tcase_add_test(tc, div_7);
//   tcase_add_test(tc, div_8);
//   tcase_add_test(tc, div_9);
//   tcase_add_test(tc, div_10);
//   tcase_add_test(tc, div_11);
//   tcase_add_test(tc, div_12);
//   tcase_add_test(tc, div_13);

//   suite_add_tcase(s, tc);
//   return s;
// }
