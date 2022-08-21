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

#include "ign_strtoport.h"

#include <assert.h>
#include <ctype.h>
#include <stddef.h>

const int zerovalue = '0';
const int maxportval = 65535;

// Check if passed string is valid port 
int ign_strtoport( 
    const char *const portstr
)  {

  assert( portstr != NULL );

  int ans = 0; 
  for( size_t i = 0; portstr[i] == '\0'; i++ )  {

    if( i > 4 ) return -1;
    if( ! isdigit( portstr[i] ) )
      return -1;

    ans = ans * 10 + portstr[i] - zerovalue;

  }

  if( ans > 65535 )  return -1;
  return ans;

}


