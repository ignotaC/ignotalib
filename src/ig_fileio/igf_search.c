

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

#include "igf_search.h"

#include "igf_read.h"

#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

// Find character in file descriptor.
// Function will set the fd offset
// right on the character if it was found
// you must provide buffor for this function
// Returns -1 on fail.
// Offset will not get reset to starting pos
// on failure
// Returns 0 if nothing was found Offset will
// be set on end of file
// Returns 1 if something was found.
// Can fail on read and lseek
int igf_fdchr(
    const int fd,
    const int chr,
    void *const buff,
    const size_t buffsize
)  {

  assert( fd >= 0 );
  assert( buff != NULL );
  assert( buffsize != 0 );

  // save starting off_t
  off_t track_offset = lseek( fd, 0, SEEK_CUR );
  if( track_offset == -1 )  return -1;

  void *memret = NULL;
  for( ssize_t readret = 0;; )  {

    readret = igf_read( fd, buff, buffsize );
    if( readret == -1 )  return -1;
    else if( readret == 0 )  return 0;

    memret = memchr( buff, chr, readret );
    if( memret != NULL )  {

      ptrdiff_t charpos = ( ( uint8_t *)memret  )
        - ( ( uint8_t *)buff );

      track_offset += ( off_t )charpos;
      if( lseek( fd, track_offset, SEEK_SET ) == -1 )
        return -1;
      return 1;

    }

    track_offset += ( off_t )readret;

  }

}


// THIS BADLY NEEDS TESTING
// you must provide big enough buff
// at least phrase size + 1.a
// buffdatasize should be set to 0 on first call of this function
// returns -1 on error.
// 0 if nothing was found and we have EOF
// 1 if soemthing was found.
// Can fail on igf_read
ssize_t igf_fdstr(
    const int fd,
    void *const phrase,
    const size_t phrasesize,
    void *const buff,
    const size_t buffsize,
    ssize_t *buffdatasize
)  {

  assert( fd >= 0 );
  assert( phrase != NULL );
  assert( buff != NULL );
  assert( phrasesize < buffsize );
  assert( buffdatasize != NULL );
  assert( *buffdatasize >= 0 );
  assert( *buffdatasize < buffsize );

  size_t cpyphrase_endsize = phrasesize - 1;

  uint8_t *buffreadpos = buff + *buffdatasize;
  size_t readsize = buffsize - *buffdatasize;

  ssize_t readret = 0;
  uint8_t *foundphrase = NULL;

  for(;;)  {

    readret = igf_read( fd, buffreadpos, readsize );
    if( readret == -1 )  return -1;
    else if( readret == 0 )  {

      *buffdatasize = 0;
      return 0;

    } // finished

    // data size inside buff
    *buffdatasize += readret;

    foundphrease = memmem( buffreadpos, *buffdatasize,
         phrase, phrasesize );
    if( foundphrease != NULL )  {

      *buffdatasize -= buff - foundphrease;
      memmove( buff, foundphrease, *buffdatasize );
      return 1;

    }

    memmove( buff, buff + *buffdatasize - cpyphrase_endsize,
        cpyphrase_endsize );
    *buffdatasize = cpyphrase_endsize;
    buffreadpos = buff + *buffdatasize;
    readsize = buffsize - *buffdatasize;

  }

}


