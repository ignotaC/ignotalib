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




// Writes bytes to fd from buff untill
// error on read or all bytes written.
// Returns 0 on succes and -1 on fail
// If unable to write - will end up in endless loop,
// write returning all the time 0.
// It can fail on write.
// Restarts on EINTR
// At error we do not know how much we have written
// On return success 0 - all bytes have been written
ssize_t igf_write(
    const int fd,
    void *const buff,
    size_t buffsize
)  {
 
  assert( fd >= 0 );
  assert( buff != NULL );
  assert( buffsize != 0 );

  const uint8_t *buffptr = buff;
  ssize_t  writeret = 0;
  
  for(;;)  {
    
    writeret = write( fd, buffptr, buffsize );
    switch( writeret )  {

      case -1:
        switch( errno )  {

	  case EINTR:
           continue;

	  default:
	   return -1;

	}

      default:
	break;

    }

    buffptr += writeret;
    buffsize -= ( size_t )writeret;
    if( buffsize == 0 )  return 0;
    
  }
    
}

