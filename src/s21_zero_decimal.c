#include "s21_decimal.h"

void s21_zero_big_decimal(s21_big_decimal* dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = dst->bits[4] =
      dst->bits[5] = dst->bits[6] = dst->bits[7] = 0;
}

/// @brief Обнуление decimal
/// @param dst
// Зануление малой структуры
void s21_zero_decimal(s21_decimal* dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
}