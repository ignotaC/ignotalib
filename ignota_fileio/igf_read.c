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



// Reads bytes from fd to buff untill error on read
// EOF or buff is full.
// Returns read bytes size or 0 if EOF.
// -1 is returned on error ( read error ).
// Restarts on EINTR
ssize_t igf_read(
  const int fd,
  void *const buff,
  size_t buffsize )  {
  
  uint8_t *buffptr = buff;
  ssize_t  readret = 0;
  ssize_t  readsum = 0;

  for(;;)  { 
    
    readret = read( fd, buffptr, buffsize );
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
    buffsize -= ( size_t )readret;
    if( ! buffsize )  break;
    
  }
  
  return readsum;
  
}



