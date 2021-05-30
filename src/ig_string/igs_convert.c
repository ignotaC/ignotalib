/*

Copyright (c) 2021 Piotr Trzpil  p.trzpil@protonmail.com

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

#include "igs_convert.h"

#include <assert.h>
#include <ctype.h>

// All alpha characters are turned to upper case,
// untill nul character met or passed_strlen passed.
// Does not work for locales only pure ascii.
// I use here igr suffix so no ub on
// str nameing. Since str* reserved.
// This function does not return anything.
void igs_strtoupper(
    char *const igs_str,
    const size_t igs_strlen
)  {

  assert( igs_str != NULL );
  assert( igs_strlen != 0 );

  for( size_t i = 0; i < igs_strlen; i++ )  {

    if( igs_str[i] == 0 )   return;
    igs_str[i] = toupper( igs_str[i] );

  }

}

// All alpha characters are turned to upper case,
// untill nul character met or passed_strlen passed.
// Does not work for locales only pure ascii.
// I use here igr suffix so no ub on
// str nameing. Since str* reserved.
// This function does not return anything.
void igs_strtolower(
    char *const igs_str,
    const size_t igs_strlen
)  {

  assert( igs_str != NULL );
  assert( igs_strlen != 0 );

  for( size_t i = 0; i < igs_strlen; i++ )  {

    if( igs_str[i] == 0 )   return;
    igs_str[i] = tolower( igs_str[i] );

  }

}


