#include "s21_decimal.h"

void s21_printf_decimal(s21_decimal d) {
  int bit = 0;
  for (int i = 127; i >= 0; i--) {
    bit = s21_get_bit(d, i);
    printf("%d", bit);
    if (i == 127 || i == 112 || i == 120 || i == 96 || i == 64 || i == 32) {
      printf(" ");
    }
  }
  printf("\n");
}