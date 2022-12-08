#include "s21_decimal.h"

/// @brief Сложение
/// @param value_1
/// @param value_2
/// @param result
/// @return 0 - OK 1 - число слишком велико или равно бесконечности 2 - число
/// слишком мало или равно отрицательной бесконечности
// Сложение с нормализацией
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int error = 0;
  // Если оба числа имеют знак "-", устанавливаем в result знак
  if (s21_get_sign(&value_1) && s21_get_sign(&value_2)) {
    s21_set_sign(result);
  }
  // Если числа разнознаковые, мы передаем их в функцию вычитания, учитывая,
  // какое число имеет отрицательный знак
  if (s21_get_sign(&value_1) != s21_get_sign(&value_2)) {
    int sign = 0;
    sign = s21_get_sign(&value_1);
    s21_set_bit(&value_1, 127, 0);
    s21_set_bit(&value_2, 127, 0);
    error = sign ? s21_sub(value_2, value_1, result)
                 : s21_sub(value_1, value_2, result);
  } else {
    s21_big_decimal v1 = {0}, v2 = {0}, r = {0};
    // Копируем данные в большой децимал
    s21_import_to_big_decimal(value_1, &v1);
    s21_import_to_big_decimal(value_2, &v2);
    int diff = s21_get_scale(&value_1) - s21_get_scale(&value_2);
    diff > 0 ? s21_set_scale(&value_2, s21_get_scale(&value_1))
             : s21_set_scale(&value_1, s21_get_scale(&value_2));
    s21_big_normalization(&v1, &v2, diff);
    s21_add_big_decimal(v1, v2, &r);
    s21_import_to_small_decimal(result, r);
    s21_set_scale(result, s21_get_scale(&value_1));
  }
  if (error) s21_zero_decimal(result);
  if (error == 1 && s21_get_scale(result)) error = 2;
  return error;
}

// Сложение в формате большого децимала
void s21_add_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal* result) {
  int res = 0, ovf = 0;
  for (int i = 0; i < 256; i++) {
    res = s21_get_bit_big(value_1, i) + s21_get_bit_big(value_2, i) + ovf;
    ovf = res / 2;
    s21_set_bit_big(result, i, res % 2);
  }
}

/// @brief Вычитание
/// @param value_1
/// @param value_2
/// @param result
/// @return 0 - OK 1 - число слишком велико или равно бесконечности 2 - число
/// слишком мало или равно отрицательной бесконечности
// Вычитание с нормализацией
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int error = 0;
  if (s21_get_sign(&value_1) && s21_get_sign(&value_2)) {
    s21_decimal temporary1 = value_1;
    value_1 = value_2;
    value_2 = temporary1;
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
      s21_decimal temporary2 = value_1;
      value_1 = value_2;
      value_2 = temporary2;
      s21_set_sign(result);
    }
    s21_big_decimal v1 = {0}, v2 = {0}, r = {0};
    s21_import_to_big_decimal(value_1, &v1);
    s21_import_to_big_decimal(value_2, &v2);
    int diff = s21_get_scale(&value_1) - s21_get_scale(&value_2);
    s21_big_normalization(&v1, &v2, diff);
    s21_sub_big_decimal(v1, v2, &r);
    s21_import_to_small_decimal(result, r);
    s21_set_scale(result, s21_get_scale(&value_1));
  }
  if (error) s21_zero_decimal(result);
  if (error == 1 && s21_get_sign(result)) error = 2;
  return error;
}

// Вычитание в формате большого децимала
void s21_sub_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal* result) {
  int tmp = 0, res = 0 /*, ovf = 0*/;
  for (int i = 0; i < 256; i++) {
    res = s21_get_bit_big(value_1, i) - s21_get_bit_big(value_2, i) - tmp;
    tmp = res < 0;
    res = abs(res);
    s21_set_bit_big(result, i, res % 2);
  }
}

/// @brief Смещение при умножении
/// @param dst
/// @param num
/// @return
int s21_shift_dec_left(s21_decimal* dst, int num) {
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

// / @brief Умножение
// / @param value_1
// / @param value_2
// / @param result
// / @return 0 - OK 1 - число слишком велико или равно бесконечности 2 - число
// / слишком мало или равно отрицательной бесконечности
// Умножение в формате большого децимала
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int error = 0 /*, sign = 0*/, scale = 0;
  s21_big_decimal v1 = {0}, v2 = {0}, r = {0};
  s21_import_to_big_decimal(value_1, &v1);
  s21_import_to_big_decimal(value_2, &v2);
  // if (s21_get_sign(&value_1) != s21_get_sign(&value_2)) sign = 1;
  scale = s21_get_scale(&value_1) + s21_get_scale(&value_2);
  s21_set_scale(result, scale);
  error = s21_mul_big_decimal(v1, v2, &r);
  s21_import_to_small_decimal(result, r);
  return error;
}

int s21_mul_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal* result) {
  int error = 0, count = 0;
  for (int i = 0; i < 256 && !error; i++) {
    if (s21_get_bit_big(value_2, i)) {
      error = s21_shift_big_dec_left(&value_1, i - count);
      s21_add_big_decimal(value_1, *result, result);
      count = i;
    }
  }
  return error;
}

// int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
//   int count = 0, error = 0, sign = 0;
//   if (s21_get_sign(&value_1) != s21_get_sign(&value_2)) sign = 1;
//   int scale = s21_get_scale(&value_1) + s21_get_scale(&value_2);
//   s21_set_scale(result, scale);
//   for (int i = 0; i < 96 && !error; i++) {
//     if (s21_get_bit(value_2, i)) {
//       error = s21_shift_dec_left(&value_1, i - count);
//       s21_add(value_1, *result, result);
//       count = i;
//     }
//   }
//   s21_set_bit(result, 127, sign);
//   if (error == 1 && s21_get_sign(result)) error = 2;
//   if (error)
//     result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] =
//     0;
//   return error;
// }

// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
//   int error = 0;
//   return error;
// }
