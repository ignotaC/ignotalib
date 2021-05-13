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


// rot 13 gets decoded using on it rgainot13 ;-)
// This function does not fail
// And it does not return anything
void ige_rot13(
    void *const buff,
    const size_t buffsize
)  {
  
  assert( buff != NULL );
  assert( buffsize != 0 );

  uint8_t *const buffptr = buff;
  
  int character = 0;
  for( size_t i = 0; i < buffsize; i++ )  {
    
    character = ( int )( buffptr[i] );
    if( ! isalpha( character ) )  continue;

    if( character <= 'M' ) character += 13;
    else if( character <= 'Z' ) character -= 13;
    else if( character <= 'm' ) character += 13;
    else  character -= 13;
    
  }
  
}
