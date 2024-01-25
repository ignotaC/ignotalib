/*

Copyright (c) 2024 Piotr Trzpil  p.trzpil@protonmail.com

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

#include "igp_allocprintf.h"

#include <errno.h>

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
 
enum flags {

  F_ZERO = 0,
  F_HASH,
  F_SPACE,
  F_PLUS,
  F_HYPEN,
  F_APOSTROPH,
  FLAGS_LEN
   
};
 
enum field {

  FIELD_NONE = 0,
  FIELD_ASTERISK,
  FIELD_DECIMAL

};
 
enum precision {

  PRECISION_NONE = 0,
  PRECISION_ASTERISK = 0,
  PRECISION_DECIMAL

};
 
 
enum lengthmode {

  LENGTHMODE_NONE = 0,
  LENGTHMODE_l,
  LENGTHMODE_ll,
  LENGTHMODE_h,
  LENGTHMODE_hh,
  LENGTHMODE_j,
  LENGTHMODE_z,
  LENGTHMODE_t,
  LENGTHMODE_L

};
 

// let us be realistic you rather won't pass huge number like that in format
// If you want full capacity use asterisk -> *
static const size_t maxdecimal = SIZE_MAX / 12;

// Does not try to recover from allocation fail
char* igp_allocprintf(
    size_t *alocstr_size,
    char *const format,
    ...
)  {

  // Our data for allocated string
  *alocstr_size = 0;
  char *alocstr = NULL;

  // size of format parts and conversion specifiers
  size_t partsize = 0;  

  for( char *pos = format;;)  {

    // look for % - conversion specifier
    char *pos_convspec = strchr( pos, '%' );

    // nothing found, allocate left format piece and leave
    if( pos_convspec == NULL )  {

      partsize = strlen( pos ) + 1; // + 1 is final nul
      alocstr = realloc( alocstr, *alocstr_size + partsize );
      if( alocstr == NULL )  return NULL;
      strncpy( &alocstr[ *alocstr_size ], pos, partsize );
      *alocstr_size += partsize;
      return alocstr;  // job, finished

    }

    // we did find conversion specifier
    // copy everything from last pos to just before specifier
    partsize = pos_convspec - pos;
    alocstr = realloc( alocstr, *alocstr_size + partsize );
    if( alocstr == NULL )  return NULL;
    memcpy( &alocstr[ *alocstr_size ], pos, partsize );
    *alocstr_size += partsize;

    pos = pos_convspec;

    // %%, put % and move on
    if( pos[1] == '%' )  {

      alocstr = realloc( alocstr, *alocstr_size + 1 );
      if( alocstr == NULL )  return NULL;
      alocstr[ *alocstr_size ] = '%';
      ( *alocstr_size )++;

      pos += 2; 
      continue;

    }

    // flags setup
    // flags can appear in any order
   int flag[ FLAGS_LEN ] = { 0 };

    // flags should not be repeated
    // find all flags
    for( ;;pos++ )  {

      switch( *pos )  {

	case '0':
	  if( flag[ F_ZERO ] )  goto brokenformat;
	  flag[ F_ZERO ] = 1;
	  continue;

	case '#':
	  if( flag[ F_HASH ] )  goto brokenformat;
	  flag[ F_HASH ] = 1;
	  continue;

	case ' ':
	  if( flag[ F_SPACE ] )  goto brokenformat;
	  flag[ F_SPACE ] = 1;
	  continue;

	case '+':
	  if( flag[ F_PLUS ] )  goto brokenformat;
	  flag[ F_PLUS ] = 1;
	  continue;

	case '-':
	  if( flag[ F_HYPEN ] )  goto brokenformat;
	  flag[ F_HYPEN ] = 1;
	  continue;

	case '\'':
	  if( flag[ F_APOSTROPH ] )  goto brokenformat;
	  flag[ F_APOSTROPH ] = 1;
	  continue;

	default:
	  break;

      }

      break;

    }

    // field setup
    // either asterisk or number
    int field = FIELD_NONE;
    size_t field_size = 0;
    // get field 
    
    // ASTERISK
    if( *pos == '*' )  {

      field = FIELD_ASTERISK;
      pos++;

    }
    else if( isdigit( *pos ) )  {

      field = FIELD_DECIMAL;

      for(;;)  {

	// guard for overflow
	if( maxdecimal < field_size )
	  goto brokenformat;

	// create field size
	field_size = field_size * 10 + ( *pos - '0' );

	pos++;
	if( !isdigit( *pos ) )  break;

      }

    }

    // precision setup
    // first appears dot - then either asterisk or number
    int precision = PRECISION_NONE;
    size_t precision_size = 0;
    
    if( *pos == '.' )  {
	
      pos++;
      // if not decimal it's asterisk so we can set as decimal
      // and change later
      precision = PRECISION_DECIMAL;

      // ASTERISK
      if( *pos == '*' )  {

        precision = PRECISION_ASTERISK;
        pos++;

      }
      else if( isdigit( *pos ) )  {

        for(;;)  {

	  // guard for overflow
	  if( maxdecimal < precision_size )
	    goto brokenformat;

	  // create field size
	  precision_size = precision_size * 10 + ( *pos - '0' );

	  pos++;
  	  if( !isdigit( *pos ) )  break;

        }

      }

      // if nothing than we treat as if precision is zero
      // so we set decimal flag ( already did that at start )
      // and precision_size stays 0

    }

    // lenght modifier setup
    int lenmod = LENGTHMODE_NONE;

    // we can only have one type
    switch( *pos )  {

      case 'l':
        lenmod = LENGTHMODE_l;
        if( ( *( pos + 1 ) ) == 'l' )  {

 	  pos++;
          lenmod = LENGTHMODE_ll;
	
        }
        break;

      case 'h':
        lenmod = LENGTHMODE_h;
        if( ( *( pos + 1 ) ) == 'h' )  {

	  pos++;
          lenmod = LENGTHMODE_hh;
	
        }
        break;

      case 'j':
        lenmod = LENGTHMODE_j;
        break;

      case 'z':
        lenmod = LENGTHMODE_z;
        break;

      case 't':
        lenmod = LENGTHMODE_t;
        break;

      case 'L':
        lenmod = LENGTHMODE_L;
        break;

      default:
        break;


    }

    pos++;

    // conversion specifier setup
    // we can only have one type
    switch( *pos )  {

      case 'd':
        break;

      case 'i':
        break;

      case 'o':
        break;

      case 'u':
        break;

      case 'x':
        break;

      case 'X':
        break;

      case 'f':
        break;

      case 'F':
        break;

      case 'e':
        break;

      case 'E':
        break;

      case 'g':
        break;

      case 'G':
        break;

      case 'a':
        break;

      case 'A':
        break;

      case 'c':
        break;

      case 's':
        break;

      case 'p':
        break;

      case 'n':
        break;

      case 'C':
        break;

      case 'S':
        break;

        // if none of above this is error!
      default:
        goto brokenformat;


    }

    pos++;
    // and we move on


  }

 brokenformat:
  free( alocstr );
  *alocstr_size = 0;
  errno = 0;  // so we know that error is from format
  return NULL;

}




