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



size_t igf_ivbuff_sumsize( const struct iovec *const iv, const int iv_len )  {

  size_t ivbuff_sumsize = 0;
  for( int i = 0; i < iv_len; i++ )  ivbuff_sumsize += iv[i].iov_len;
  return ivbuff_sumsize;
  
}


ssize_t igf_readv( const int fd, struct iovec *const iv, int iv_len, const size_t size_to_read )  {

  ssize_t readv_ret;
  size_t read_sum = 0;

  struct iovec *const iv_ptr = iv;
  size_t keep_len = iv_ptr->iov_len;
  void *keep_ptr = iv_ptr->iov_base;
  
  while( size_to_read )  {

    readv_ret = readv( fd, iv_ptr, iv_len );

    if( readv_ret < 0 )  {
      
      if( errno == EINTR )  continue;
      
      iv_ptr->iov_len = keep_len;
      iv_ptr->iov_base = keep_ptr;
      return -1;
      
    }
    else if( read_ret == 0 )  {
      
      iv_ptr->iov_len = keep_len;
      iv_ptr->iov_base = keep_ptr;
      return read_sum;
      
    }

    read_sum += readv_ret;
    
    if( size_to_read == read_sum )  {

      iv_ptr->iov_len = keep_len;
      iv_ptr->iov_base = keep_ptr;
      return read_sum;

    }
  
    for(;;)  {

      readv_ret -= iv_ptr->iov_len;
      if( readv_ret <= 0 )  {

	iv_ptr->iov_len += readv_ret;
	iv_ptr->iov_base -= readv_ret;
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


// if want to write less cheat with iv_summary_size and give smaller
ssize_t igf_writev( const int fd, struct iovec *const iv, const int iv_len, const size_t write_size )  {

  int changed_pos = 0;
  void *changed_ptr = iv->iov_base;
  size_t *changed_len = iv->iov_len;
  
  struct iovec *iv_pos = iv->iov_base;
  int iv_left_len = iv_len;
  ssize_t write_return;

  int i = 0;
  while( ( write_return = ( TEMP_FAILURE_RETRY( writev( fd, iv_pos, iv_left_len ) ) ) ) > 0 )  {

    iv[ changed_pos ].iov_base = changed_ptr;
    iv[ changed_pos ].iov_len = changed_len;
    
    while( write_return )  {

      write_return -= iv_pos->iov_len; // TO JEST DO ROZWIĄZANIA. BLAD FAIL.
      if( write_return < 0 )  {  // problem co jak przerwało w  TEJ SAMEJ STRUKTURZE.

	chnaged_pos = iv_pos->iov_base;
	chnaged_len = iv_pos->iov_len;

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


