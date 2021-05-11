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



FIX THIS

// Writes bytes to fd from buff untill error on read or all bytes written.
// Returns 0 on succes and -1 on fail.
// If unable to write will end up in endless loop ( write returning all the time 0
// When trying to write somethign still ).
// Restarts on EINTR
// Tested, satisfied.
ssize_t igf_write( const int fd, void *const passed_buff, size_t passed_buff_size )  {
  
  const uint8_t *buff_ptr = passed_buff;
  ssize_t  write_return = 0;
  
  for(;;)  {
    
    if( ( write_return = ( TEMP_FAILURE_RETRY( write( fd, buff_ptr, passed_buff_size ) ) ) ) <= 0 )  {
      
      if( write_return == 0 )  {
        
        if( passed_buff_size == 0 )  return 0;
        continue;
        
      }
      
      return write_return;
      
    }
    
    buff_ptr += write_return;
    passed_buff_size -= ( size_t )write_return;

    if( passed_buff_size == 0 )  return 0;
    
  }
    
}

