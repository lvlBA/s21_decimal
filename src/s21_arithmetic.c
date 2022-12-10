#include "s21_decimal.h"

// Сложение с нормализацией
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int error = 0;
  // Если оба числа имеют знак "-", устанавливаем в result знак
  if (s21_get_sign(value_1) && s21_get_sign(value_2)) {
    s21_set_sign(result);
  }
  // Если числа разнознаковые, мы передаем их в функцию вычитания, учитывая,
  // какое число имеет отрицательный знак
  if (s21_get_sign(value_1) != s21_get_sign(value_2)) {
    int sign = 0;
    sign = s21_get_sign(value_1);
    s21_set_bit(&value_1, 127, 0);
    s21_set_bit(&value_2, 127, 0);
    error = sign ? s21_sub(value_2, value_1, result)
                 : s21_sub(value_1, value_2, result);
  } else {
    s21_big_decimal v1 = {0}, v2 = {0}, r = {0};
    // Копируем данные в большой децимал
    s21_import_to_big_decimal(value_1, &v1);
    s21_import_to_big_decimal(value_2, &v2);
    int diff = s21_get_scale(value_1) - s21_get_scale(value_2);
    diff > 0 ? s21_set_scale(&value_2, s21_get_scale(value_1))
             : s21_set_scale(&value_1, s21_get_scale(value_2));
    s21_normalization(&v1, &v2, diff);
    s21_add_big_decimal(v1, v2, &r);
    s21_import_to_small_decimal(result, r);
    s21_set_scale(result, s21_get_scale(value_1));
  }
  if (error) s21_zero_decimal(result);
  if (error == 1 && s21_get_scale(*result)) error = 2;
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

// Вычитание с нормализацией
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int error = 0;
  if (s21_get_sign(value_1) && s21_get_sign(value_2)) {
    s21_decimal temporary1 = value_1;
    value_1 = value_2;
    value_2 = temporary1;
    s21_set_bit(&value_1, 127, 0);
    s21_set_bit(&value_2, 127, 0);
  }
  if (s21_get_sign(value_1) != s21_get_sign(value_2)) {
    s21_get_sign(value_1) ? s21_set_sign(result) : 1;
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
    int diff = s21_get_scale(value_1) - s21_get_scale(value_2);
    s21_normalization(&v1, &v2, diff);
    s21_sub_big_decimal(v1, v2, &r);
    s21_import_to_small_decimal(result, r);
    s21_set_scale(result, s21_get_scale(value_1));
  }
  if (error) s21_zero_decimal(result);
  if (error == 1 && s21_get_sign(*result)) error = 2;
  return error;
}

// Вычитание в формате большого децимала
void s21_sub_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal* result) {
  int tmp = 0, res = 0;
  for (int i = 0; i < 256; i++) {
    res = s21_get_bit_big(value_1, i) - s21_get_bit_big(value_2, i) - tmp;
    tmp = res < 0;
    res = abs(res);
    s21_set_bit_big(result, i, res % 2);
  }
}

// Умножение в формате большого децимала
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int error = 0, /* sign = 0,*/ scale = 0;
  s21_big_decimal v1 = {0}, v2 = {0}, r = {0};
  s21_import_to_big_decimal(value_1, &v1);
  s21_import_to_big_decimal(value_2, &v2);
  // if (s21_get_sign(value_1) != s21_get_sign(value_2)) sign = 1;
  scale = s21_get_scale(value_1) + s21_get_scale(value_2);
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

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int error = 0, /*bit_1 = 0, bit_2 = 0,*/ scale = 0, res_scale = 0;
  s21_big_decimal v1 = {0}, v2 = {0}, r = {0};
  s21_import_to_big_decimal(value_1, &v1);
  s21_import_to_big_decimal(value_2, &v2);
  // Определяем расположение старшего бита в числах
  // s21_find_highest_bit_decimal(value_1, value_2, &bit_1, &bit_2);
  // Основное действие деления в большом децимале
  scale = s21_div_big_decimal(v1, v2, &r);
  s21_set_scale(&value_1, s21_get_scale(value_1) + scale);
  res_scale = s21_get_scale(value_1) - s21_get_scale(value_2);
  if (res_scale > 0)
    res_scale = s21_post_normalization(&r, res_scale);
  else
    s21_increase_scale_big_decimal(&r, abs(res_scale));
  s21_import_to_small_decimal(result, r);
  s21_set_scale(result, res_scale);
  return error;
}

int s21_div_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                        s21_big_decimal* result) {
  int b_1 = 0, b_2 = 0, bit_2 = 0, scale = 0, diff = 0, save_scale = 0;
  s21_big_decimal tmp = {0};
  // Определяем старший бит в структурах
  s21_find_highest_bit_big_decimal(value_1, value_2, &b_1, &b_2);
  bit_2 = b_2;
  for (int i = 0; i < 96 && s21_is_big_decimal_not_empty(value_1);) {
    if (i > 0) {
      s21_shift_big_dec_left(&value_2, 1);
      s21_increase_scale_big_decimal(result, 1);
      s21_increase_scale_big_decimal(&value_1, 1);
      save_scale++;
    }
    // Выравнивание по битам
    scale = s21_equation_bits_big_decimal(&value_1, &value_2);
    save_scale += scale;
    b_1 = b_2 = 0;
    s21_find_highest_bit_big_decimal(value_1, value_2, &b_1, &b_2);
    diff = b_2 - bit_2;
    if (diff < 0) diff = 0;
    for (; diff >= 0 && s21_is_big_decimal_not_empty(value_1);) {
      if (s21_is_greater_big_decimal(value_2, value_1)) {
        s21_set_bit_big(&tmp, 0, 0);
      } else {
        s21_sub_big_decimal(value_1, value_2, &value_1);
        s21_set_bit_big(&tmp, 0, 1);
      }
      i++;
      diff--;
      if (diff >= 0) s21_shift_big_dec_right(&value_2, 1);
      s21_shift_big_dec_left(&tmp, 1);
    }
    if (diff >= 0) s21_shift_big_dec_left(&tmp, diff + 1);
    s21_shift_big_dec_right(&tmp, 1);
    s21_add_big_decimal(*result, tmp, result);
    s21_zero_big_decimal(&tmp);
  }
  return save_scale;
}

/******************* Additional functions ******************/

// Перенос данных из малой в большую структуру
void s21_import_to_big_decimal(s21_decimal value_1, s21_big_decimal* value_2) {
  value_2->bits[0] = value_1.bits[0];
  value_2->bits[1] = value_1.bits[1];
  value_2->bits[2] = value_1.bits[2];
}

void s21_import_to_small_decimal(s21_decimal* value_1,
                                 s21_big_decimal value_2) {
  value_1->bits[0] = value_2.bits[0];
  value_1->bits[1] = value_2.bits[1];
  value_1->bits[2] = value_2.bits[2];
}

// Установить бит в большой структуре
void s21_set_bit_big(s21_big_decimal* dst, int index, int bit) {
  int mask = 1u << (index % 32);
  if (bit == 0)
    dst->bits[index / 32] = dst->bits[index / 32] & ~mask;
  else
    dst->bits[index / 32] = dst->bits[index / 32] | mask;
}

// Получение бита в большой структуре
int s21_get_bit_big(s21_big_decimal dst, int index) {
  int mask = 1u << (index % 32);
  return (dst.bits[index / 32] & mask) != 0;
}

// Сдвиг big decimal вправо +
void s21_shift_big_dec_right(s21_big_decimal* dst, int num) {
  // int bit = 0;
  int buffer[7] = {0};
  for (int k = 0; k < num; k++) {
    for (int i = 0; i < 7; i++) {
      buffer[i] = s21_get_bit_big(*dst, (i + 1) * 32);
    }
    for (int i = 0; i < 7; i++) {
      dst->bits[i] >>= 1;
      s21_set_bit_big(dst, i * 32 - 1, buffer[i]);
    }
    dst->bits[7] >>= 1;
  }
}

// Сдвиг big decimal влево +
int s21_shift_big_dec_left(s21_big_decimal* dst, int num) {
  int error = 0 /*, bit = 0*/;
  int buffer[8] = {0};
  for (int k = 0; k < num; k++) {
    for (int i = 0; i < 7; i++) {
      buffer[i] = s21_get_bit_big(*dst, (i + 1) * 32 - 1);
    }
    for (int i = 7; i > 0 && !error; i--) {
      if (s21_get_bit_big(*dst, 255)) error = 1;
      dst->bits[i] <<= 1;
      s21_set_bit_big(dst, i * 32, buffer[i - 1]);
    }
    dst->bits[0] <<= 1;
  }
  return error;
}

// Увеличение scale decimal на n
void s21_increase_scale_decimal(s21_decimal* dst, int n) {
  s21_decimal tmp = {0}, ten = {{10, 0, 0, 0}};
  int scale = s21_get_scale(*dst);
  for (int i = 0; i < n; i++) {
    s21_mul(*dst, ten, &tmp);
    *dst = tmp;
    s21_zero_decimal(&tmp);
  }
  s21_set_scale(dst, scale + n);
}

// Увеличение scale big decimal на n
void s21_increase_scale_big_decimal(s21_big_decimal* dst, int n) {
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}}, tmp = {0};
  for (int i = 0; i < n; i++) {
    s21_mul_big_decimal(*dst, ten, &tmp);
    *dst = tmp;
    s21_zero_big_decimal(&tmp);
  }
}

void s21_decreace_scale_big_decimal(s21_big_decimal* dst, int n) {
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}}, tmp = {0};
  for (int i = 0; i < n; i++) {
    s21_div_big_decimal(*dst, ten, &tmp);
    *dst = tmp;
    s21_zero_big_decimal(&tmp);
  }
}

int s21_is_greater_big_decimal(s21_big_decimal value_1,
                               s21_big_decimal value_2) {
  int result = 0;
  for (int i = 7; i >= 0 && !result; i--) {
    if (value_1.bits[i] > value_2.bits[i]) {
      result = 1;
    }
  }
  return result;
}

int s21_is_greater_or_equal_big_decimal(s21_big_decimal value_1,
                                        s21_big_decimal value_2) {
  int result = 0, out = 0;
  for (int i = 7; i >= 0 && !out && !result; i--) {
    if (value_1.bits[i] != 0 || value_2.bits[i] != 0) {
      if (value_1.bits[i] >= value_2.bits[i]) {
        result = 1;
      }
      out = 1;
    }
  }
  return result;
}

int s21_is_big_decimal_not_empty(s21_big_decimal dst) {
  return dst.bits[0] + dst.bits[1] + dst.bits[2] + dst.bits[3] + dst.bits[4] +
         dst.bits[5] + dst.bits[6] + dst.bits[7];
}

void s21_find_highest_bit_decimal(s21_decimal v1, s21_decimal v2, int* bit_1,
                                  int* bit_2) {
  for (int i = 95; i >= 0 && (!(*bit_1) || !(*bit_2)); i--) {
    if (*bit_1 == 0 && s21_get_bit(v1, i)) *bit_1 = i;
    if (*bit_2 == 0 && s21_get_bit(v2, i)) *bit_2 = i;
  }
}

void s21_find_highest_bit_big_decimal(s21_big_decimal v1, s21_big_decimal v2,
                                      int* bit_1, int* bit_2) {
  for (int i = 255; i >= 0 && (!(*bit_1) || !(*bit_2)); i--) {
    if (*bit_1 == 0 && s21_get_bit_big(v1, i)) *bit_1 = i;
    if (*bit_2 == 0 && s21_get_bit_big(v2, i)) *bit_2 = i;
  }
}

int s21_equation_bits_big_decimal(s21_big_decimal* value_1,
                                  s21_big_decimal* value_2) {
  int scale = 0;
  while (s21_is_greater_big_decimal(*value_2, *value_1)) {
    s21_increase_scale_big_decimal(value_1, 1);
    scale++;
  }
  while (s21_is_greater_or_equal_big_decimal(*value_1, *value_2)) {
    s21_shift_big_dec_left(value_2, 1);
  }
  s21_shift_big_dec_right(value_2, 1);
  return scale;
}
/* ********************** BIG DECIMAL ************************* */

// int main(void) {
//   s21_decimal v1 = {{1234, 0, 0, 0}};
//   // s21_set_scale(&v1, 8);
//   s21_decimal v2 = {{12340, 0, 0, 0}};
//   s21_decimal res = {0};
//   s21_div(v1, v2, &res);
//   printf("[%u] [%u] [%u] [%u]\n", res.bits[0], res.bits[1], res.bits[2],
//          res.bits[3]);
//   float f = 0;
//   s21_from_decimal_to_float(res, &f);
//   return 0;
// }
