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


#include "igc_probability.h"

#include <stdio.h>
#include <string.h>

// Function initialize igc_dict struct.
// It does not return anything.
void igc_init_dict(
    struct igc_dict *const dp
)  {

  assert( dp != NULL );

  dp->len = 0;
  dp->wp = NULL;
  dp->count = 0;

}

// Function initializes igc_word struct.
// It does not return anything.
void igc_init_word(
    struct igc_word *const wp 
)  {

  assert( wp != NULL );

  wp->word = NULL;
  wp->wordlen = 0;
  wp->probability = 0.0;
  wp->binary_expression = NULL;
  wp->binarylen = 0;
  wp->count = 0;

}

// Allocate new word inside the igc_word struct.
// This function can fail on realloc or malloc
// It won't loose it's contents on fail.
// NULL is returned on fail, else if success
// return the pointer to new filled
// igc_word struct.
// Added word does not get counted.
struct igc_word *igc_addword_dict(
    struct igc_dict *const dp,
    const char *const word,
    const size_t wordlen
)  {

  assert( dp != NULL );
  assert( word != NULL );
  assert( wordlen != 0 );

  char *newword = malloc( wordlen );
  if( newword == NULL )  return NULL;

  struct igc_word *wp = realloc( dp->wp,
    sizeof( dp->wp ) * ( dp->len + 1 ) );

  if( wp == NULL )  {

     free( newword );	  
     return NULL;

  }
  dp->wp = wp;
  // Now go to clean wp position
  wp = &wp[ dp->len ]; // which is on our old len value
  igc_init_word( wp );
  wp->word = newword;
  memcpy( wp->word, word, wordlen );
  wp->wordlen = wordlen;
  ( dp->len )++;
  return wp;

}

// Function takes passed word and andds it to
// dictionary or it simply counts it.
// It can only fail on realloc/malloc,
// inside the igc_addword_dict function.
// Even if it fails it keeps it's contents.
// Without any modification
// Still out of memory is usually end of program
// anyway.
// -1 is returned on fail, 0 when function success.
int igc_recvword_dict(
    struct igc_dict *const dp,
    const char *const word,
    const size_t wordlen
)  {

  assert( dp != NULL );
  assert( word != NULL );
  assert( wordlen != 0 );

  struct igc_word *wp = NULL;
  for( size_t i = 0; i < dp->len; i++ )  {

    wp = dp->wp;

    // It assums the longest words are first
    // in the dictionary list - word order.
    if( wordlen != wp->wordlen )  continue;
    if( memcmp( wp->word, word, wordlen ) )  continue;

    wp->count++;
    dp->count++;
    return 0;

  }

  // still if not found add it.
  wp = igc_addword_dict( dp, word, wordlen );
  if( wp == NULL )  return -1;

  wp->count++;
  dp->count++;
  return 0;

}

// Function does not return anything
// It frees igc_dicte struct contents
void igc_free_dict(
    struct igc_dict *const dp
)  {

  assert( dp != NULL );

  // First free all the contents of wp
  for( size_t i = 0; i < dp->len; i++ )  {

    free( ( &dp->wp[i] )->word );

  }

  free( dp->wp );

}


// Function prints contents of igc_dict struct.
// It can fail on puts, and igc_print_word.
// If it fails it return -1.
// On success it returns 0.
int igc_print_dict( 
    const struct igc_dict *const dp
)  {

  assert( dp != NULL );

  if( printf( "Summary count: %" PRIu64 "\n",
     dp->count ) < 0 )
  
  for( size_t i = 0; i < dp->len; i++ )  {

    if( igc_print_word( &dp->wp[i] ) == -1 )
      return -1;
    if( puts("") == EOF )  return -1;

  }

  return 0;

}

// Function prints contents of igc_word struct
// It can fail on printf, if it fails it return -1
// On success it returns 0
int igc_print_word(
    const struct igc_word *const wp
)  {

  assert( wp != NULL );

  if( printf( "Word: %s\n", wp->word ) < 0 )
    return -1;

  if( printf( "Word length: %zu\n", wp->wordlen ) < 0 )
    return -1;
  
  if( printf( "Probability: %f\n", wp->probability )
    < 0 )  return -1;
  
  if( printf( "Binary expression: %s\n",
    wp->binary_expression ) < 0 )  return -1;
  
  if( printf( "Binary length: %d\n", wp->binarylen )
    < 0 )  return -1;
  
  if( printf( "Count: %" PRIu64 "\n", wp->count ) < 0 )
    return -1;

}

// Function counts probability for 
// igc_word struct
void igc_findprob_dict(
    struct igc_dict *const dp
)  {

  assert( dp != NULL );

  struct igc_word *wp = NULL;
  
  for( size_t i = 0; i < dp->len; i++ )  {

    wp = &( dp->wp[i] );
    wp->probability = ( double )wp->count / dp->count;

  }

}




