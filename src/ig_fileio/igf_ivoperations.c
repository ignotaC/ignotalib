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

#include "igf_ivoperations.h"

#include <assert.h>
#include <errno.h>
#include <stddef.h>


size_t igf_ivbuff_sumsize(
    const struct iovec *const iv,
    const int iv_len
)  {

  assert( iv != NULL );
  assert( iv_len > 0 );

  size_t ivbuff_sumsize = 0;
  for( int i = 0; i < iv_len; i++ )
    ivbuff_sumsize += iv[i].iov_len;
  return ivbuff_sumsize;
  
}


ssize_t igf_readv(
    const int fd, 
    struct iovec *const iv, 
    int iv_len,
    const size_t readsize
)  {

  assert( fd >= 0 );
  assert( iv != NULL );
  assert( iv_len > 0 );
  assert( readsize != 0 );

  ssize_t readvret;
  size_t readv_sum = 0;

  struct iovec *iv_ptr = iv;
  size_t keep_len = iv_ptr->iov_len;
  void *keep_ptr = iv_ptr->iov_base;
  
  for(;;)  {

    readvret = readv( fd, iv_ptr, iv_len );

    if( readvret < 0 )  {
      
      if( errno == EINTR )  continue;
      
      iv_ptr->iov_len = keep_len;
      iv_ptr->iov_base = keep_ptr;
      return -1;
      
    }
    else if( readvret == 0 )  {
      
      iv_ptr->iov_len = keep_len;
      iv_ptr->iov_base = keep_ptr;
      return readv_sum;
      
    }

    readv_sum += readvret;
    
    if( readsize == readv_sum )  {

      iv_ptr->iov_len = keep_len;
      iv_ptr->iov_base = keep_ptr;
      return readv_sum;

    }
  
    for(;;)  {

      readvret -= iv_ptr->iov_len;
      if( readvret <= 0 )  {

	iv_ptr->iov_len += readvret;
	iv_ptr->iov_base -= readvret;
	break;
	
      }

      iv_ptr->iov_len = keep_len;
      iv_ptr->iov_base = keep_ptr;

      iv_ptr++;
      iv_len--;
      
      keep_len = iv_ptr->iov_len;
      keep_ptr = iv_ptr->iov_base;
      
    }
      
  }
    
}


// This is so broken
// This is broken - look for writesize in code doing something - non existant
// There is bug here


ssize_t igf_writev( 
    const int fd,
    struct iovec *const iv, 
    const int iv_len, 
    const size_t writesize
)  {

  assert( fd >= 0 );
  assert( iv != NULL );
  assert( iv_len > 0 );
  assert( writesize != 0 );

  int changed_pos = 0;
  void *changed_ptr = iv->iov_base;
  size_t changed_len = iv->iov_len;
  
  struct iovec *iv_pos = iv->iov_base;
  int iv_left_len = iv_len;
  ssize_t write_return;

  int i = 0;
  // handle EINTR
  while( ( write_return =  writev( fd, iv_pos, iv_left_len ) )  > 0 )  {

    iv[ changed_pos ].iov_base = changed_ptr;
    iv[ changed_pos ].iov_len = changed_len;
    
    while( write_return )  {

      write_return -= iv_pos->iov_len; // TO JEST DO ROZWIĄZANIA. BLAD FAIL.
      if( write_return < 0 )  {  // problem co jak przerwało w  TEJ SAMEJ STRUKTURZE.

	changed_pos = iv_pos->iov_base;
	changed_len = iv_pos->iov_len;

	iov_pos->iov_base = ( uint8t* )iov_pos->iov_base - write_return;
	iov_pos->iov_len += write_return;	
	break;

      }

      iv_pos++;
      iv_left_len--;

    }
    
  }

  iv[ changed_pos ].iov_base = changed_ptr;
  iv[ changed_pos ].iov_len = changed_len;
  
  if( write_return < 0 )  return -1;
  return 0;
  
}


