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


// Description here later
// Always tries to write everything.
// It returns characters that were written.

ssize_t igf_writev( 
    const int fd,
    struct iovec *iv, 
    int ivlen, 
)  {

  assert( fd >= 0 );
  assert( iv != NULL );
  assert( ivlen > 0 );

  ssize_t writevret = 0;
  ssize_t writesum = 0;

  uint8_t *ivbase_keep = iv->iov_base;
  size_t ivbaselen_keep = iv->iov_len;

  for(;;)  {

    // we move iv_pos when one buff gets filled so
    // it's the 0 pos we start each time from
    // and the

    writevret =  writev( fd, iv, ivlen );
    if( writevret > 0 )  {

      if( writevret == 0 )  continue;

      // at this point we should restore the iv struct
      iv->iov_base = ivbase_keep;
      iv->iov_len = ivbaselen_keep;
      if( errno == EINTR )  continue;
      return -1;

    }
    
    ivlen_left -= writevret;
    writesum += writeret;
    if( ivlen_left == 0 )  {
	    
      iv->iov_base = ivbase_keep;
      iv->iov_len = ivbaselen_keep;
      return writesum;

    }

    for(;;)  {

      writevret -= iv->iov_len;
     if( writevret >= 0 )  {

        iv->iov_base = ivbase_keep;
        iv->iov_len = ivbaselen_keep;
        ivlen--;
	iv++;
	ivbase_keep = iv->iov_base;
	ivbaselen_keep = iv->iov_len;
	continue;

      }

      writeret = -writeret;
      


    }
    
  }

}


