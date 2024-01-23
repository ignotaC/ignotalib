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

#include "igf_write.h"
#include "../ig_time/igt_sleep.h"

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>


// Writes bytes to fd from buff untill
// error on read or all bytes written.
// Returns 0 on succes and -1 on fail
// If unable to write - will end up in endless loop,
// It can fail on write.
// Restarts on EINTR
// At error we do not know how much we have written
// On return success 0 - all bytes have been written
ssize_t igf_write(
    const int fd,
    void *const buff,
    size_t writesize
)  {
 
  assert( fd >= 0 );
  assert( buff != NULL );
  assert( writesize != 0 );

  const uint8_t *buffptr = buff;
  ssize_t  writeret = 0;
  
  for(;;)  {
    
    writeret = write( fd, buffptr, writesize );
    if( writeret == -1 )  {

        switch( errno )  {

	  case EINTR:
           continue;

	  default:
	   return -1;

	}

    }

    buffptr += writeret;
    writesize -= ( size_t )writeret;
    if( ! writesize )  return 0;
    
  }
    
}

// Writes bytes to fd from buff untill
// error on read or all bytes written.
// Returns 0 on succes and -1 on fail
// If unable to write - will end up in endless loop,
// It can fail on write.
// Restarts on EINTR
// On EAGAINJ EWOULDBLOCK waiting specified miliseconds
// than try to write again
// At error we do not know how much we have written
// On return success 0 - all bytes have been written
ssize_t igf_writeall_nb(
    const int fd,
    void *const buff,
    size_t writesize,
    const long wait_milisec
)  {
 
  assert( fd >= 0 );
  assert( buff != NULL );
  assert( writesize != 0 );

  const uint8_t *buffptr = buff;
  ssize_t  writeret = 0;
  
  for(;;)  {
    
    writeret = write( fd, buffptr, writesize );
    if( writeret == -1  )  {

        switch( errno )  {

	  #if ( EWOULDBLOCK != EAGAIN )
	  case EWOULDBLOCK:
	  #endif
	  case EAGAIN:
	   igt_sleepmilisec( wait_milisec );	
	  case EINTR:
           continue;

	  default:
	   return -1;

	}

    }

    buffptr += writeret;
    writesize -= ( size_t )writeret;
    if( ! writesize )  return 0;
    
  }
    
}

