#include "s21_decimal.h"
// void test_float(float num1, float num2, int orig);
// void test_int(int num1, int num2, int orig);
int main() {
  s21_decimal val1 = {{0}};
  s21_decimal val2 = {{0}};
  s21_set_bit(&val1, 127, 1);
  s21_set_bit(&val2, 127, 1);
  s21_set_bit(&val1, 83, 1);
  s21_set_bit(&val2, 83, 1);
  s21_set_scale(&val1, 10);
  s21_set_scale(&val2, 11);
  // s21_set_scale(&value_2, 2);
  int result = s21_is_less(val1, val2);
  printf_decimal(val1);
  printf_decimal(val2);

  printf("%d\n", result);
  // float f1 = 1.375342323523;
  // float f2 = 1.39;
  // test_float(f1, f2, 1);

  // int n1 = 3;
  // int n2 = 9;
  // test_int(n1, n2, 1);

  // n1 = -3;
  // n2 = -3;
  // test_int(n1, n2, 0);

  // // 3766
  // s21_decimal src1 = {0}, src2 = {0};
  // // src1 = 32323465788987654;
  // // src2 = 67543278.89765424354657687;

  // src1.bits[0] = 0b01011000010000110001100100000110;
  // src1.bits[1] = 0b00000000011100101101011000000101;
  // src1.bits[2] = 0b00000000000000000000000000000000;
  // src1.bits[3] = 0b00000000000000000000000000000000;

  // src2.bits[0] = 0b10111110000100011000010110010111;
  // src2.bits[1] = 0b11011001001111010001001011000110;
  // src2.bits[2] = 0b00000000000001011001011001001000;
  // src2.bits[3] = 0b00000000000100010000000000000000;
  // // src2.bits[3] = 0b00000000000000000000000000000000;
  // int result = s21_is_less_or_equal(src1, src2);
  // int origin = 0;
  // printf("res = %d, orig = %d\n", result, origin);
  return 0;
}

// void test_float(float num1, float num2, int orig) {
//   s21_decimal dec1, dec2;
//   s21_from_float_to_decimal(num1, &dec1);
//   s21_from_float_to_decimal(num2, &dec2);
//   int res = s21_is_less(dec1, dec2);
//   printf("res = %d, orig = %d\n", res, orig);
// }
// void test_int(int num1, int num2, int orig) {
//   s21_decimal dec1, dec2;
//   s21_from_int_to_decimal(num1, &dec1);
//   s21_from_int_to_decimal(num2, &dec2);
//   int res = s21_is_less(dec1, dec2);
//   printf("res = %d, orig = %d\n", res, orig);
// }
/*
gcc -Wall -Wextra -Werror test.c s21_converters.c s21_get_set.c s21_comparison.c
s21_normalize.c s21_arithmetic.c s21_zero_decimal.c
*/