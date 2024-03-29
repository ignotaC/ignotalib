

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

#include <stdio.h>

#include <assert.h>
#include <stddef.h>

// short options initalizator - should be called at all times.
void igmisc_sopts_init(
    igmisc_short_opts *const sopts,
    const unsigned char *permitted
)  {

  assert( sopts != NULL );
  assert( permitted != NULL );
  
  // set all option not permited to -1
  for( int i = 0; i < IGMISC_SOPTMAX_SIZE; i++ )
    ( *sopts )[i] = -1;
  
  // set permitted to 0
  while( *permitted != '\0' )  {

    ( *sopts )[ *permitted ] = 0;
    permitted++; 

  }

}

// Short options start with '-' else return -1
// At least one  character but not nul must appear
// after '-' else return -1
// If option is not allowed ( -1 ) return -1
// Otherwise it's incremented.
// No errors = return 0
int igmisc_sopts_readcount(
    igmisc_short_opts *const sopts,
    const unsigned char *optstr 
)  {

  assert( sopts != NULL );
  assert( optstr != NULL );
 
  if( optstr[0] != '-' )  return -1;
  if( optstr[1] == '\0' )  return -1;

  optstr++;
  do  {

    if( ( *sopts )[ *optstr ] == -1 )
      return -1;
    ( ( *sopts )[ *optstr ] )++;
    optstr++;

  }  while( *optstr != '\0' );

  return 0;

}

// Short options start with '-' else return -1
// At least one  character but not nul must appear
// after '-' else return -1
// If option is not allowed ( -1 ) return -1
// Otherwise it's order of appearing is set
// If option appears again nothing is done
// No errors = return 0
// BUG - when  -4 -6 it will not detect what was first opt order.
// TODO ^^
int igmisc_sopts_readorder(
    igmisc_short_opts *const sopts,
    const unsigned char *optstr 
)  {

  assert( sopts != NULL );
  assert( optstr != NULL );
 
  if( optstr[0] != '-' )  return -1;
  if( optstr[1] == '\0' )  return -1;

  int order = 1;

  optstr++;
  do  {

    if( ( *sopts )[ *optstr ] == -1 )
      return -1;

    if( ( ( *sopts )[ *optstr ] ) == 0 )  {

      ( ( *sopts )[ *optstr ] ) = order;
      order++;  

    }
    // else it already appeared and we do nothing
    optstr++;

  }  while( *optstr != '\0' );

  return 0;

}

// load all short options from passed
// argument strings that you expect to contain options
// also need to pass function for options read
int igmisc_sopts_load(
    igmisc_short_opts *const sopts,
    int ( *sopts_read )( igmisc_short_opts *const, const unsigned char* ),
    int optc,
    const unsigned char *optv[]
)  {

  assert( sopts != NULL );
  assert( sopts_read != NULL );
  assert( optv != NULL );
 
  for( int i = 0; i < optc; i++ )  {

    if( sopts_read( sopts, optv[i] ) == -1 )
      return -1;

  }

  return 0;

}


// simply print options that appeared
int igmisc_sopts_print(
    igmisc_short_opts *const sopts,
    const unsigned char *permitted
)  {

  assert( sopts != NULL );
  assert( permitted != NULL );
  
  while( *permitted != '\0' )  {

    if( ( *sopts )[ *permitted ] != 0 )  {

      if( printf( "%c", ( int )( *permitted ) ) < 0 )
        return -1;

    }
    permitted++; 

  }

  return 0;

}



