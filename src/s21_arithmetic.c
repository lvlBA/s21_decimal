#include "s21_decimal.h"

/// @brief Сложение
/// @param value_1
/// @param value_2
/// @param result
/// @return 0 - OK 1 - число слишком велико или равно бесконечности 2 - число
/// слишком мало или равно отрицательной бесконечности
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int tmp = 0, res = 0, error = 0;
  s21_normalize(&value_1, &value_2);
  if (s21_get_sign(&value_1) && s21_get_sign(&value_2)) {
    s21_set_sign(result);
  }
  if (s21_get_sign(&value_1) != s21_get_sign(&value_2)) {
    int sign = s21_get_sign(&value_1);
    s21_set_bit(&value_1, 127, 0);
    s21_set_bit(&value_2, 127, 0);
    error = sign ? s21_sub(value_2, value_1, result)
                 : s21_sub(value_1, value_2, result);
  } else {
    for (int i = 0; i < 3; i++) {
      for (int j = (i * 31 + i); j <= 31 * (i + 1) + i; j++) {
        res = s21_get_bit(value_1, j) + s21_get_bit(value_2, j) + tmp;
        if (j == 95 && res > 1) error = 1;
        tmp = res / 2;
        s21_set_bit(result, j, res % 2);
      }
    }
  }
  if (error == 1 && s21_get_sign(result)) error = 2;
  if (error) s21_zero_decimal(result);
  return error;
}

/// @brief Вычитание
/// @param value_1
/// @param value_2
/// @param result
/// @return 0 - OK 1 - число слишком велико или равно бесконечности 2 - число
/// слишком мало или равно отрицательной бесконечности
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int tmp = 0, res = 0, error = 0;
  if (s21_get_sign(&value_1) && s21_get_sign(&value_2)) {
    s21_decimal temporary = value_1;
    value_1 = value_2;
    value_2 = temporary;
    s21_set_bit(&value_1, 127, 0);
    s21_set_bit(&value_2, 127, 0);
  }
  if (s21_get_sign(&value_1) != s21_get_sign(&value_2)) {
    s21_get_sign(&value_1) ? s21_set_sign(result) : 1;
    s21_set_bit(&value_1, 127, 0);
    s21_set_bit(&value_2, 127, 0);
    error = s21_add(value_1, value_2, result);
  } else {
    if (s21_is_greater_or_equal(value_2, value_1)) {
      s21_decimal temporary = value_1;
      value_1 = value_2;
      value_2 = temporary;
      s21_set_sign(result);
    }
    for (int i = 0; i < 3; i++) {
      for (int j = (i * 31 + i); j < 31 * (i + 1) + i; j++) {
        res = s21_get_bit(value_1, j) - s21_get_bit(value_2, j) - tmp;
        tmp = res < 0;
        res = abs(res);
        s21_set_bit(result, j, res % 2);
      }
    }
  }
  if (error == 1 && s21_get_sign(result)) error = 2;
  if (error) s21_zero_decimal(result);
  return error;
}

/// @brief Смещение при умножении
/// @param dst
/// @param num
/// @return
int s21_shift_dec_left(s21_decimal *dst, int num) {
  int error = 0, bit = 0;
  for (int i = 0; i < num && !error; i++) {
    if (s21_get_bit(*dst, 95)) {
      error = 1;
    } else {
      dst->bits[2] <<= 1;
      bit = s21_get_bit(*dst, 63);
      dst->bits[1] <<= 1;
      s21_set_bit(dst, 64, bit);
      bit = s21_get_bit(*dst, 31);
      dst->bits[0] <<= 1;
      s21_set_bit(dst, 32, bit);
    }
  }
  return error;
}

/// @brief Умножение
/// @param value_1
/// @param value_2
/// @param result
/// @return 0 - OK 1 - число слишком велико или равно бесконечности 2 - число
/// слишком мало или равно отрицательной бесконечности
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int count = 0, error = 0, sign = 0;
  if (s21_get_sign(&value_1) != s21_get_sign(&value_2)) sign = 1;
  int scale = s21_get_scale(&value_1) + s21_get_scale(&value_2);
  s21_set_scale(result, scale);
  for (int i = 0; i < 96 && !error; i++) {
    if (s21_get_bit(value_2, i)) {
      error = s21_shift_dec_left(&value_1, i - count);
      s21_add(value_1, *result, result);
      count = i;
    }
  }
  s21_set_bit(result, 127, sign);
  if (error == 1 && s21_get_sign(result)) error = 2;
  if (error)
    result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  return error;
}

/// Деление
