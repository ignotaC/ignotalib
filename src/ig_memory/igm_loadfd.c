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

#include "igm_loadfd.h"

#include "../ig_fileio/igf_read.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

// Loads fd to memory.
// The fd_pos of file is set back on it's right place.
// Which means if we read it a bit.
// It will be just there where we where reading it.
// Can fail on lseek, read and malloc.
// At error the old fdpos is set back to starting value
// Returns memory pointer where the file is stored.
// On fail returns NULL.
// On fail it's UNDEFINED the function managed to
// set back the pointer back or not.
// You must pass buffor and it's size for this function
void* igm_fdtomem(
    const int fd,
    size_t *const memsize,
    void *const buff,
    const size_t buffsize )  {
  
  assert( fd >= 0 );
  assert( memsize != NULL );
  assert( buff != NULL );
  assert( buffsize != 0 );

  off_t keep_fdpos = lseek( fd, 0, SEEK_CUR );
  if( keep_fdpos == -1 )   return NULL;
  
  off_t end_fdpos = lseek( fd, 0, SEEK_END );
  if( end_fdpos == -1 )   goto errorstart;

// Now go at start of file.
  off_t read_fdpos = lseek( fd, 0, SEEK_SET );
  if( read_fdpos == -1 )   goto errorstart;

  *memsize = ( size_t )end_fdpos - read_fdpos;
  void *mem = malloc( *memsize );
  if( mem == NULL )  goto erroralloc;

  uint8_t *mempos = mem;
  
  ssize_t readret = 0;
  while( ( readret = igf_read( fd, buff, buffsize ) )
      > 0 )  {

    memcpy( mempos, buff, ( size_t )readret );
    mempos += readret;
    
  }
  if( readret != 0 )  goto errorread;


  read_fdpos = lseek( fd, keep_fdpos, SEEK_SET );
  if( read_fdpos == -1 )   goto errorseek;
  return mem;

 errorseek:
 errorread:
  free( mem );
 erroralloc:
 errorstart:;
  int keeperr = errno;
  // still try to set this back
  lseek( fd, keep_fdpos, SEEK_SET );
  errno = keeperr;
  return NULL;

}



