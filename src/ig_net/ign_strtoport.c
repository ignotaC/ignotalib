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

const int zerocharvalue = '0';
const int maxportval = 65535;

// Check if passed string is valid port 
// -1 on WRONG port
// else port  number is returned
// chances are probably 0 that int will be 16 bit but
// long int to being strict
long int ign_strtoport( 
    const char *const portstr
)  {

  assert( portstr != NULL );

  // special case, else ans 0 whill break
  if( portstr[0] == '\0' )  return -1;

  long int ans = 0; // what iff "" is passed? we will get -1 
		// this way so error, as should be
  
  for( size_t i = 0; portstr[i] != '\0'; i++ )  {

    if( i > 4 ) return -1;  // too big for a port number
    if( ! isdigit( portstr[i] ) )  // must be digits alone
      return -1;

    ans = ans * 10 + portstr[i] - zerocharvalue; // count port

  }

  if( ans > 65535 )  return -1;
  return ans;

}
