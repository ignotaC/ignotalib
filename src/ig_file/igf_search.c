

/*

Copyright (c) 2021-2022 Piotr Trzpil  p.trzpil@protonmail.com

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

// for bigmem in future - look for matching few  or one  character
// then check whole string.


// This function will always reset offset to found  memory
// if nothing is found - value of offset is set back to 
// offset value before any read
// Memory we look for is smaller than buff. 
// If not, than the behavior is undefined.
// 0 if nothing was found and we have EOF
// 1 if soemthing was found.
// -1 fail and errno set.
off_t igf_findmem(
    const int fd,
    void *const mem,
    const size_t memsize,
    void *const buff,
    const size_t buffsize
)  {

  // check for  most simple mistakes
  assert( fd >= 0 );
  assert( mem != NULL );
  assert( buff != NULL );
  assert( buffsize > 0 );
  assert( buffsize > memsize );

  // If memsize is zero than simply return
  // without doing anything
  if( memsize == 0 )  return 0;

  // keep the start offset
  off_t save_offset = lseek( fd, 0, SEEK_CUR );
  if( save_offset == -1 )  return -1;

  // we need one byte so we can move pointer
  // also to keep the mem and buff pointer untouched
  // it is constant anyway
  uint8_t *buffp = buff;
  uint8_t *foundpos;

  // We need this since some part of buff 
  // will remain from earlier read
  size_t buffsize_left = buffsize;
  size_t buff_readsize = 0;

  // this is used at the end of function loop
  // static value 
  const size_t tailsize = memsize - 1;

  // set up return value for read etc
  for( ssize_t readret;;)  {

    //read data into buffp - but pass the filled memory
    readret = igf_read( fd, &buffp[ buff_readsize ],
       buffsize - buff_readsize );
    if( readret == -1 )  return -1;
    else if( readret == 0 )  {

      // reset the offset back to value before read
      if( lseek( fd, save_offset, SEEK_SET ) == -1 )
        return -1;

      return 0;  // finished, nothing was found

    }

    // count how much actual data we have inside buff
    buff_readsize += ( size_t )readret;

    // if buff_readsize is smaller then mem size we
    // should try to read more 
    if( buff_readsize < memsize )
      continue;

    // look for the memory  
    foundpos = memmem( buffp, buff_readsize, mem, memsize );

    // we found something so just return the proper offset
    // and function is done
    if( foundpos != NULL )
        return lseek( fd, foundpos - buffp - buff_readsize, SEEK_CUR );

    // nothing found, move tail data in buff at start of it.
    // Obviously we need to keep last characters  in size of memsize
    // without one char , since buff_readsize sis position
    // after our last character in buff it's a -1 + 1
    // so it simplifies to ust as we see down below 
    memmove( buffp, &buffp[ buff_readsize ] - memsize, tailsize );
    buff_readsize = tailsize;

  }

}

