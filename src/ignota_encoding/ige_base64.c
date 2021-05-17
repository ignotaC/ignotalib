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


static const char ige_base64_enctab[] = 
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghi"
  "jklmnopqrstuvwxyz0123456789+/";

// Encode base64
// Works only for small endian.
// You must be sure that str_encoded is big enought or it will return error.
// ( ( x / 3 ) * 4 ) + 5   |||  4 bytes more might be lost on /3 + place for nul = 5  or just use ig_getcoded_base64size  function
// Never fails but can overflow.  
void* ige_encbase64(
    const void *const bytes, 
    const size_t bytes_size,
    void *const coded_buff )  {
 
  assert( bytes != NULL );
  assert( bytes_size != 0 );
  assert( coded_buff != NULL );

  uint8_t *const coded_str = coded_buff;
  const uint8_t *bytes_ptr = ( uint8_t* ) bytes;
  uint16_t storage = 0;
  uint8_t *storage_ptr = ( uint8_t* ) &storage;
    
  size_t j = 0, i = 0, n = bytes_size;
  for(; n > 2; n -= 3 )  {

    // xx xx xx xx 00 00 00 00
    storage_ptr[1] = bytes_ptr[i++];
    // 00 xx xx xx
    coded_str[j++] = ig_encode_base64_tab[ ( storage_ptr[1] ) >> 2 ];

    // xx xx xx xx xx xx xx xx 
    storage_ptr[0] = bytes_ptr[i++];
    // xx xx xx xx xx 00 00 00
    storage <<= 6;
    // 00 xx xx xx    
    coded_str[j++] = ig_encode_base64_tab[ ( storage_ptr[1] ) >> 2 ];
 
    // xx xx xx xx 00 00 00 00
    storage <<= 2;
    // xx xx xx xx xx xx xx xx 
    storage_ptr[0] = bytes_ptr[i++];
    // xx xx xx xx xx xx 00 00 
    storage <<= 4;
    // 00 xx xx xx    
    coded_str[j++] = ig_encode_base64_tab[ ( storage_ptr[1] ) >> 2 ];    
    // xx xx xx 00 00 00 00 00
    storage <<= 6;
    // 00 xx xx xx    
    coded_str[j++] = ig_encode_base64_tab[ ( storage_ptr[1] ) >> 2 ];
     
  }  
  
  // 'n' are left chcm to encode
  
  if( n == 2 )  {
    
    storage_ptr[1] = bytes_ptr[i++];
    coded_str[j++] = ig_encode_base64_tab[ ( storage_ptr[1] ) >> 2 ];

    storage_ptr[0] = bytes_ptr[i++];
    storage <<= 6;
    coded_str[j++] = ig_encode_base64_tab[ ( storage_ptr[1] ) >> 2 ];
 
    storage <<= 2;
    storage_ptr[0] = 0;
    storage <<= 4;
    coded_str[j++] = ig_encode_base64_tab[ ( storage_ptr[1] ) >> 2 ];
    coded_str[j++] = '=';
    
  }
  
  if( n == 1 )  {
    
    storage_ptr[1] = bytes_ptr[i++];
    coded_str[j++] = ig_encode_base64_tab[ ( storage_ptr[1] ) >> 2 ];

    storage_ptr[0] = 0;
    storage <<= 6;
    coded_str[j++] = ig_encode_base64_tab[ ( storage_ptr[1] ) >> 2 ];
    coded_str[j++] = '=';
    coded_str[j++] = '=';
    
  }
  
  coded_str[j++] = 0;
  return coded_str;
  
}

// NOT tested, return safe size of string after coding it in base.
// in base we turn 3 bytes to 4
// + 1 -> we need place for nul.
// worst case we end up not using all bytes but this is a small memory lost.
// We want here the safe size without getting into details.
// 
size_t ige_base64len( const size_t encoded_size )  {

  return encoded_size / 3 * 4 + 1;

}
