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

#include "ige_base64.h"

#include <assert.h>
#include <stdint.h>

static const char ige_base64_ctab[] = 
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghi"
  "jklmnopqrstuvwxyz0123456789+/";

// Code base64
// Works only for small endian.
// You must be sure that str_encoded is big enought or it will return error.
// ( ( x / 3 ) * 4 ) + 5   |||  4 bytes more might be lost on /3 + place for nul = 5  or just use ig_getcoded_base64size  function
// Never fails but can overflow.  
void* ige_cbase64(
    const void *const mem, 
    const size_t memsize,
    void *const codedmem
)  {
 
  assert( mem != NULL );
  assert( memsize != 0 );
  assert( codedmem != NULL );

  uint8_t *const codedptr = codedmem;
  const uint8_t *memptr = ( uint8_t* ) mem;
  uint16_t storage = 0;
  uint8_t *storageptr = ( uint8_t* ) &storage;
    
  size_t j = 0, i = 0, n = memsize;
  for(; n > 2; n -= 3 )  {

    // xx xx xx xx 00 00 00 00
    storageptr[1] = memptr[i++];
    // 00 xx xx xx
    codedptr[j++] = ige_base64_ctab[ ( storageptr[1] ) >> 2 ];

    // xx xx xx xx xx xx xx xx 
    storageptr[0] = memptr[i++];
    // xx xx xx xx xx 00 00 00
    storage <<= 6;
    // 00 xx xx xx    
    codedptr[j++] = ige_base64_ctab[ ( storageptr[1] ) >> 2 ];
 
    // xx xx xx xx 00 00 00 00
    storage <<= 2;
    // xx xx xx xx xx xx xx xx 
    storageptr[0] = memptr[i++];
    // xx xx xx xx xx xx 00 00 
    storage <<= 4;
    // 00 xx xx xx    
    codedptr[j++] = ige_base64_ctab[ ( storageptr[1] ) >> 2 ];    
    // xx xx xx 00 00 00 00 00
    storage <<= 6;
    // 00 xx xx xx    
    codedptr[j++] = ige_base64_ctab[ ( storageptr[1] ) >> 2 ];
     
  }  
  
  // 'n' are left chcm to encode
  
  if( n == 2 )  {
    
    storageptr[1] = memptr[i++];
    codedptr[j++] = ige_base64_ctab[ ( storageptr[1] ) >> 2 ];

    storageptr[0] = memptr[i++];
    storage <<= 6;
    codedptr[j++] = ige_base64_ctab[ ( storageptr[1] ) >> 2 ];
 
    storage <<= 2;
    storageptr[0] = 0;
    storage <<= 4;
    codedptr[j++] = ige_base64_ctab[ ( storageptr[1] ) >> 2 ];
    codedptr[j++] = '=';
    
  }
  
  if( n == 1 )  {
    
    storageptr[1] = memptr[i++];
    codedptr[j++] = ige_base64_ctab[ ( storageptr[1] ) >> 2 ];

    storageptr[0] = 0;
    storage <<= 6;
    codedptr[j++] = ige_base64_ctab[ ( storageptr[1] ) >> 2 ];
    codedptr[j++] = '=';
    codedptr[j++] = '=';
    
  }
  
  codedptr[j++] = 0;
  return codedptr;
  
}

// NOT tested, return safe size of string after coding it in base.
// in base we turn 3 bytes to 4
// + 1 -> we need place for nul.
// worst case we end up not using all bytes but this is a small memory lost.
// We want here the safe size without getting into details.
//  ige befor ename because of str reserved.
size_t ige_base64size(
    const size_t ige_strsize
)  {

  return ige_strsize / 3 * 4 + 1;

}
