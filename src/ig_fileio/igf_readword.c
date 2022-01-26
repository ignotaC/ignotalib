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

#include "igf_readword.h"

#include "igf_fdoffset.h"
#include "igf_read.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Read word from fd 
// if end of file met it will return  ""
// so simply return[0] = '\0' 
// Function returns allocated word.
// empty string on EOF must also be freed.
char* igf_readword(
  const int fd,
  void *const buff,
  const size_t buffsize
)  {

  assert( fd >= 0 );
  assert( buff != NULL );
  assert( buffsize != 0 );

  uint8_t *bpos = buff;
  uint8_t *word_begin = NULL;

  char *word = NULL;
  char *save_wordptr = NULL;

  bool fin = 0;
  size_t alloc_size = 0, oldalloc_size = 0,
	 cpy_size = 0;

  for( ssize_t i = 0;; i = 0 )  {

    ssize_t readsize = igf_read( fd, buff, buffsize );
    if( readsize == -1 )  goto freeonerr;
    if( readsize == 0 )  {

      // empty string on  EOF
      if( word == NULL )
        word = strdup( "" );

      // We kind of did not expect EOF
      // so we must alloc one more byte for nul
      save_wordptr = word;
      realloc( word, alloc_size + 1 );
      if( word == NULL )  {

        word = save_wordptr;
	goto freeonerr;

      }
      word[ alloc_size ] = '\0';
      return word; // if error we would return NULL anyway

    }

    if( word_begin == NULL )  {

      for(; i < readsize; i++ )  {

	if( bpos[i] == '\0' )  continue;      
        if( isspace( bpos[i] ) )  continue;

        word_begin = &bpos[i];
	break;

      }

      // since all were spaces continue reading.
      if( word_begin == NULL )  continue;

    }

    for(; i < readsize; i++ )  {

      if( ( bpos[i] == '\0' ) || ( isspace( bpos[i] ) ) ) {

	// move to whitespace/nul offset
	if( igf_fdoffset_mv( fd, i - readsize ) == -1 )
          goto freeonerr;
	fin = true;
        break;

      }

    }
    
    oldalloc_size = alloc_size;
    cpy_size = &bpos[i] - word_begin;
    alloc_size += cpy_size;
    if( fin )  alloc_size++;  // nul

    save_wordptr = word;
    word = realloc( word, alloc_size );
    if( word == NULL )  {

      word = save_wordptr;
      goto freeonerr;

    }
    
    memcpy( &word[ oldalloc_size ], word_begin, cpy_size );
    if( fin )  {
	
      word[ alloc_size - 1 ] = '\0';
      return word;  

    }

    // now we only look for ending spaces and nul
    // copying all the stuff.
    word_begin = buff; 

  }

freeonerr:
  free( word );
  return NULL;

}



