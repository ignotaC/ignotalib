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

#include "igf_read.h"

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

// Reads bytes from fd to buff untill error on read
// EOF or buff is full.
// Returns read bytes size or 0 if EOF.
// -1 is returned on error ( read error ).
// Handles EINTR
ssize_t igf_read(
  const int fd,
  void *const buff,
  size_t readsize
)  {
 
  assert( fd >= 0 );
  assert( buff != NULL );
  assert( readsize != 0 );

  uint8_t *buffptr = buff;
  ssize_t  readret = 0;
  ssize_t  readsum = 0;

  for(;;)  { 
    
    readret = read( fd, buffptr, readsize );
    switch( readret )  {

      case -1:
       switch( errno )  {

         case EINTR:
           continue;

	 default:
           return -1;

       }

      case 0:
       return readsum;

      default:
       break;

    }
    
    readsum += readret;
    buffptr += readret;
    readsize -= ( size_t )readret;
    if( ! readsize )  break;
    
  }
  
  return readsum;
  
}


// Reads bytes from fd to buff untill error on read
// EOF or buff is full.
// errno set to zero always so we can detect EOF from EAGAIN
// Returns read bytes size or 0 if EOF.
// -1 is returned on error ( read error ).
// Handles on EINTR
// On EAGAIN or EWOULDBLOCK it shall stop reading
// simply return what you have read as far.
ssize_t igf_read_nb(
  const int fd,
  void *const buff,
  size_t readsize
)  {
 
  assert( fd >= 0 );
  assert( buff != NULL );
  assert( readsize != 0 );

  errno = 0;

  uint8_t *buffptr = buff;
  ssize_t  readret = 0;
  ssize_t  readsum = 0;

  for(;;)  { 
    
    readret = read( fd, buffptr, readsize );
    switch( readret )  {

      case -1:
       switch( errno )  {

         case EINTR:
           continue;

         #if ( EWOULDBLOCK != EAGAIN )
         case EWOULDBLOCK:
         #endif
         case EAGAIN:
           return readsum;

	 default:
           return -1;

       }

      case 0:
       return readsum;

      default:
       break;

    }
    
    readsum += readret;
    buffptr += readret;
    readsize -= ( size_t )readret;
    if( ! readsize )  break;
    
  }
  
  return readsum;
  
}


