/*

Copyright (c) 2023 Piotr Trzpil  p.trzpil@protonmail.com

Permission to use, copy, modify, and distribute 
this software for any purpose with or without fee
is hereby granted, provided that the above copyright
notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR
DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE
FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
OF THIS SOFTWARE.

Prise God in holy trinity: 
Jezus Chrystus,
Duch Swiety,
Bog Ojeciec.

*/

#include "igmath_matrix.h"

// Sarrus method
static int igmath_matrix_3x3_determinant( 
    const int *const *const matrix
)  {

  int ans = 0;

  ans += matrix[0][0] * matrix[1][1] * matrix[2][2];
  ans += matrix[1][0] * matrix[2][1] * matrix[0][2];
  ans += matrix[2][0] * matrix[0][1] * matrix[1][2];

  ans -= matrix[0][2] * matrix[1][1] * matrix[2][0];
  ans -= matrix[1][2] * matrix[2][1] * matrix[0][0];
  ans -= matrix[2][2] * matrix[0][1] * matrix[1][0];

  return ans;

}
 
static int igmath_matrix_2x2_determinant( 
    const int *const *const matrix
)  {

  int ans = 0;

  ans += matrix[0][0] * matrix[1][1];
  ans -= matrix[0][1] * matrix[1][0];

  return ans;

}

// TODO assert all

static void igmath_init_matrix_minor_split(
    struct igmath_matrix_minor_split *const matrix_ms
    const int *const *const matrix,
    const size_t raw_len.
    const size_t col_len
    struct igmath_matrix_minor_split *const top_minor,
    const size_t minors_count
    const int minors_multiplier
)  {


  matrix_ms->matrix = matrix;
  matrix_ms->raw_len = raw_len;
  matrix_ms->col_len = col_len;
  matrix_ms-top_minor = top_minor
  matrix_ms->minors_count = minors_count;
  matrix_ms->minors_count = minor_multiplyer;

  matrix_ms->minor = NULL;
  matrix_ms->minors_pos = 0;
  matrix_ms->determinant = 0;

}

int igmath_matrix_determinant( 
    const int *const *const matrix,
    const size_t raw_len,
    const size_t col_len )  {

  if( raw_len != col_len )  {

    // TODO handle error
    return 0;

  }

  if( raw_len == 0 )  {

    // TODO handle error
    return 0;

  }

  if( raw_len == 1 )  {

    return matrix[0][0];

  }

  if( raw_len == 2 )  {

    return igmath_matrix_2x2_determinant( matrix );

  }

  if( raw_len == 3 )  {

    return igmath_matrix_3x3_determinant( matrix );

  }

  // matrix 4x4 or bigger
  struct igmath_matrix_minor_split matrix_ms;
  igmath_init_matrix_minor_split(
      &matrix_ms, matrix, raw_len, col_len,
      NULL, raw_len - 1, 1 );

  for(;;)  {

    // do stuff 

  }

  int ans = matrix_ms.determinant

    // freee stuff

  return ans;

}

