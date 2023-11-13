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

#include <stdlib.h>

// Sarrus method
static int igmath_matrix_3x3_determinant( 
    int *const *const matrix
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
    int *const *const matrix
)  {

  int ans = 0;

  ans += matrix[0][0] * matrix[1][1];
  ans -= matrix[0][1] * matrix[1][0];

  return ans;

}

// TODO assert all

static void igmath_matrix_minor_split_init(
    struct igmath_matrix_minor_split *const matrix_ms,
    int **const matrix,
    const size_t matrix_squareroot,
    struct igmath_matrix_minor_split *const top_minor,
    const size_t minors_count,
    const int minors_multiplier
)  {

  matrix_ms->matrix = matrix;
  matrix_ms->matrix_squareroot = matrix_squareroot;
  matrix_ms->top_minor = top_minor;
  matrix_ms->minors_count = minors_count;
  matrix_ms->minors_multiplier = minors_multiplier;

  matrix_ms->minors = NULL;
  matrix_ms->minors_pos = 0;
  matrix_ms->determinant = 0;

}

static void igmath_matrix_free(
    struct igmath_matrix_minor_split *minor 
)  {

  // free rows content
  for( size_t i = 0; i < minor->matrix_squareroot; i++ )
    free( minor->matrix[i] );

  // free matrix rows
  free( minor->matrix );

}

int igmath_matrix_determinant( 
    int **const matrix,
    const size_t squareroot )  {

  // error
  if( squareroot == 0 )  {

    // TODO handle error
    return 0;

  }

  // 1x1 matrix, determinant is the only value matrix keeps
  if( squareroot == 1 )  {

    return matrix[0][0];

  }

  // 2x2 matrix easy to get determinant
  if( squareroot == 2 )  {

    return igmath_matrix_2x2_determinant( matrix );

  }

  // 3x3 matrix easy to get determinant
  if( squareroot == 3 )  {

    return igmath_matrix_3x3_determinant( matrix );

  }

  // matrix 4x4 or bigger
  struct igmath_matrix_minor_split matrix_ms;
  struct igmath_matrix_minor_split *ms_pos = &matrix_ms;
  igmath_matrix_minor_split_init(
      &matrix_ms, matrix, squareroot,
      NULL, 0, 1 );

  for(;;)  {

    // this is minor we can count determinant with sarrus
    if( ms_pos->matrix_squareroot == 3 )  {

      // math part  
      int minor_determinant = igmath_matrix_3x3_determinant( ms_pos->matrix );
      minor_determinant *= ms_pos->minors_multiplier;

      // free 3x3 matrix - we won't be using it anymore
      igmath_matrix_free( ms_pos );

      // move to top minor
      ms_pos = ms_pos->top_minor;

      // add determinant to top minor determinant
      ms_pos->determinant += minor_determinant;

      // increment our position in minors
      ms_pos->minors_pos++;

      // restart loop
      continue;

    }

    // create minors 
    if( ms_pos->minors == NULL )  {

      // allocate minors
      size_t minors_len = ms_pos->matrix_squareroot - 1;
      ms_pos->minors = malloc( sizeof *( ms_pos->minors ) * minors_len );
      if( ms_pos->minors == NULL )  {

        // TODO handle malloc errors
        return 0;

      }

      // allocate matrix and set minors
      int multiplier_part = 1;
      for( size_t i = 0; i < minors_len; i++ )  {

        int **minor_matrix = malloc( sizeof( *minor_matrix ) * minors_len );
        if( minor_matrix == NULL )  {

          //TODO handle malloc error
          return 0;

        }

        for( size_t j = 0; j < minors_len; j++ )  {

          minor_matrix[j] = malloc( sizeof ( **minor_matrix ) * minors_len );
          if( minor_matrix[j] == NULL )  {

            // TODO handle malloc error
            return 0;

          }

        }

        // For now we assume minors are created allways from first row
        // filling stuff before column we ignore
        size_t k_minor = 0;
        for( size_t k = 0; k < i; k++ )  {

          // allways 1 because we choose allways first row
          size_t j_minor = 0;
          for( size_t j = 1; j < minors_len; j++, j_minor++ )
            minor_matrix[ k_minor ][ j_minor ] = ms_pos->matrix[k][j];

          k_minor++;

        }

        // now after i pos column we ignore
        for( size_t k = i + 1; k < minors_len; k++ )  {

          // continuing row 1
          size_t j_minor = 0;
          for( size_t j = 1; j < minors_len; j++, j_minor++ )
            minor_matrix[ k_minor ][ j_minor ] = ms_pos->matrix[k][j];

          k_minor++;

        }

        // set multiplier
        int minors_multiplier = ms_pos->matrix[0][i] * multiplier_part;
        multiplier_part *= -1;  // every column we move changes it

        // init a minor
        igmath_matrix_minor_split_init(
            &ms_pos->minors[i], minor_matrix, minors_len,
            ms_pos, minors_len, minors_multiplier 
        );

      }

    }

    // we need to leave the minor determinant
    // since we looped through all minors
    if( ms_pos->minors_pos == ms_pos->minors_count )  {

      // free the minors pointer and cleanup matrix
      free( ms_pos->minors );

      // If top minor is null
      // do not free the matrix since it's the root matrix
      // which was passed down to function
      if( ms_pos->top_minor != NULL )
        igmath_matrix_free( ms_pos );

      // perform the math to get determinant
      int determinant = ms_pos->determinant * ms_pos->minors_multiplier;

      // if top determinant is NULL we are at finish
      // return the determinant
      // by default top determinant multiplyer is simp[ly 1
      // so it has no effect on the determinant value
      if( ms_pos->top_minor == NULL )
        return determinant;

      // move to top minor
      ms_pos = ms_pos->top_minor;

      // add determinant we counted to top minor determinant
      ms_pos->determinant += determinant;

      // increment to next minor
      ms_pos->minors_pos++;

    }

    // move into next minor
    ms_pos = &ms_pos->minors[ ms_pos->minors_pos ];

  }

  // we should never reach here

}

