

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


#include "igmisc_opts.h"

#include <assert.h>
#include <stddef.h>

int igmisc_opts_get(
    igmisc_opts *const opts,
    const int optc,
    const char *const  optv[] 
);



void igmisc_opts_init(
    igmisc_opts *const opts,
    const char *permitted
)  {

  assert( opts != NULL );
  assert( permitted != NULL );
  
  // set all option not permited to -1
  for( int i = 0; i < IGMISC_SOPTMAX_SIZE; i++ )
    ( *opts )[i] = -1;
  
  // set permitted to 0
  while( *permitted != '\0' )  {

    ( *opts )[ *permitted ] = 0;
    permitted++; 

  }

}

int igmisc_opts_get(
    igmisc_opts *const opts,
    const int optc,
    const char *const  optv[] 
)  {

  return 1;

}

