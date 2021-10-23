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

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// print double, remove trailing zeros so
// 34.049000 will be printed like this: 34.049
// 35.000000 like this:  35
// can fial on snprintf allocation errors etc.
// if buff_size  is to small functon sets errno = 0 and returns -1
int igp_double_rmvtrailzeros(
    const double number,
    const int precision
)  {

  assert( precision >= 0 );

  int needed_size = snprintf( NULL, 0, "%.*f",
      precision, number );
  if( needed_size < 0 )  return -1;
  
  // +1 is our nul
  char *tempbuff = malloc( ( size_t )needed_size + 1 );
  if( tempbuff == NULL )  return -1;
  
  int snprintf_ret = snprintf( tempbuff, needed_size + 1,
      "%.*f", precision, number );

  if( snprintf_ret < 0 )  goto freeonerror;

  size_t i = 0;
  char  *dotpos = NULL;
  for(; tempbuff[i] != '\0'; i++ )  {

    if( tempbuff[i] != '.' )  continue;
    dotpos = &tempbuff[i];

  }

  if( dotpos == NULL )   {

    if( printf( "%s", tempbuff ) < 0 )  goto freeonerror;
    free( tempbuff );
    return 0;

  }

  i--; // move before nul
  for(; &tempbuff[i] != dotpos; i-- )  {
 
    if( tempbuff[i] == '.' )  {

      tempbuff[i] = '\0';
      break;

    }
    if( tempbuff[i] != '0' )  break;
    tempbuff[i] = '\0';

  }

  if( printf( "%s", tempbuff ) < 0 )  goto freeonerror;
  free( tempbuff );
  return 0;

 freeonerror:
  free( tempbuff );
  return -1;

}

