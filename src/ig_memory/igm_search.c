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

#include "igm_search.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void *igm_inmem(
    void *const bigmem,
    const size_t bm_size,
    void *const smallmem,
    const size_t sm_size
)  {

  assert( bigmem != NULL );
  assert( smallmem != NULL );

  // if sm is size zero it will allways match
  if( sm_size == 0 )  return bigmem;
  // if there is nothing in bm than we won't find anything
  if( bm_size == 0 )  return NULL;

  // if we are looking for one byte we use memchr simply
  uint8_t *smp = smallmem;
  if( sm_size == 1 )
    return memchr( bigmem, ( int )( *smp ), bm_size );

  uint8_t *bmp = bigmem, *keepstart = bigmem;
  uint8_t *mp = NULL;
  for( size_t leftsize = bm_size; leftsize >= sm_size; )  {
                            /*  sm_size can't be bigger */

    // First find one matching byte.
    mp = memchr( bmp, ( int )( *smp ), leftsize );
    if( mp == NULL )  return NULL;  
    // ^ Nothing found, great leave.

    // Check does the whole memory match.
    if( ! memcmp( mp, smp, sm_size ) )
      return mp; // Found the memory that matches.
    
    // We will move past the mp.
    // But can we ?
    if( leftsize == 1 )  return NULL;
    // ^ obviously we could have not.

    // Now we need to set new leftsize and bigmem pointer.
    // The additional 1 is because we pass the mp memchr
    // returned.
    bmp = mp + 1;
    leftsize = bm_size - ( bmp - keepstart );

  }  // search loop end

  // leftsize is smaller than sm_size,
  // we won't find anything here, return
  return NULL;

}

