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

#include "igds_strarr.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Can fail on malloc and realloc.
// Should not lose data on fail.
// On fail should not leak memory
// Returns 0 on success and -1 on fail.
// Array is for strings. You pass length
// and function adds nul at the end.
// You do not pass nul character.
// It will not look for it. Instead it simply
// copies memory length and adds nul
int igds_strarr_addent(
    struct igds_strarr *const sa,
    void *const mem,
    const size_t memlen
)  {

  assert( sa != NULL );
  assert( mem != NULL );
  assert( memlen != 0 );
  // little sens to make element that does
  // not need memory

  // set up memory
  char *newstr = malloc( sizeof **( sa->list ) 
    * memlen + 1 );
  // + 1 is for nul
  if( newstr == NULL ) return -1;
  char **newlist = realloc( sa->list,
    sizeof *( sa->list ) * ( sa->listlen + 1 ) );
  if( newlist == NULL )  {

    free( newstr );
    return -1;

  }
  
  sa->list = newlist;
  sa->list[ sa->listlen ] = newstr;
  memcpy( newstr, mem, memlen );
  newstr[ memlen ] = 0; // Set nul.
  ( sa->listlen )++;
  return 0;
  
};
