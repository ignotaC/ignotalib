/*

Copyright (c) 2022 Piotr Trzpil  p.trzpil@protonmail.com

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

#include "igmath_bignum.h"

#include <stdlib.h>
#include <string.h>

static int digit_count = 0;
void igmath_init(
    void
)  {

  uintmax_t biggest_number = UINTMAX_MAX;
  for(; biggest_number != 0; digit_count++ )
    biggest_number /= 10;

  // I need to be sure I can always add two numbers and no overflow.
  digit_count -= 1;

}

void igmath_init_bignum( 
    igmath_bignum *const bignum
)  {

  bignum->plus = true;
  bignum->val = NULL;
  bignum->val_len = 0;

}


int  igmath_loadnum(
    igmath_bignum *const bignum,
    const char *const numstr
)  {

  size_t numstr_len = strlen( numstr );
  bignum->val_len = numstr_len / digit_count;
//  bignum->val = malloc( sizeof *( bignum->val ) * ( 1 +  )
  return 1; // TODO FINISH THIS

}
