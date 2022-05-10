

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

#include "igf_offset.h"
#include "igf_read.h"
#include "igf_search.h"

#include "../ig_memory/igm_search.h"

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
int igf_findmem(
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
  // will need to be saved -> part from earlier read
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
    foundpos = igm_inmem( buffp, buff_readsize, mem, memsize );

    // we found something so just return the proper offset
    // and function is done
    if( foundpos != NULL )  {
     
      // first go back characters we have read.
      // then  when we are on buffp star tposition
      // just subtract foundposs from buffp since we now
      // must move forword to position we are looking for
      if( lseek( fd, foundpos - buffp - buff_readsize,
          SEEK_CUR ) == -1 )
        return -1;
      return 1;

    }

    // nothing found, move tail data in buff at start of it.
    // Obviously we need to keep last characters  in size of memsize
    // without one char , since buff_readsize sis position
    // after our last character in buff it's a -1 + 1
    // so it simplifies to ust as we see down below 
    memmove( buffp, &buffp[ buff_readsize ] - memsize, tailsize );
    buff_readsize = tailsize;

  }

}

// You must be sure the mem1 and mem2 sizes are smaller than buff size.
// otherwise undefined behavior.
// returns:
// 1 - found something
// set the fd position after mem1
// and set bytesnum - number of bytes
// just before mem2 position
// 0 - did not find anything
// reset fd to starting position
//
// -1 error
//  the fd position is undefined
int igf_amidmem(
    const int fd,
    void *const mem1,
    const size_t mem1size,
    void *const mem2,
    const size_t mem2size,
    void *const buff,
    const size_t buffsize,
    size_t *const bytesnum
)  {

  assert( fd >= 0 );
  assert( mem1 != NULL );
  // it makes little to zero sens 
  // to allow size of memo1 and mem2
  // to be zero here
  assert( mem1size > 0 );
  assert( mem2 != NULL );
  assert( mem2size > 0 );
  assert( buff != NULL );
  assert( buffsize > 0 );
  assert( bytesnum != NULL );

  // We will need this if  nothing shall be found.
  // the offset will be reseted to the starting
  // position when function was called.
  const off_t keepoffset = lseek( fd, 0, SEEK_CUR );
  if( keepoffset == -1 )  return -1;

  // find memory1
  int ans = igf_findmem( 
    fd, mem1, mem1size, buff, buffsize );

  if( ans == -1 )  return -1;
  // if we did not find anything do not reset
  // fd offset - igf_findmem allready did it
  //  since it did not find anything.
  if( ans == 0 )  return 0;

  // now we need to move offset
  // to pass mem1
  if( igf_offset_mv( fd, ( off_t )mem1size ) == -1 )
    return -1;

  // save the offset after the mem1
  // this is out part of answet if mem2 will be found
  const off_t aftermem1_offset = lseek( fd, 0, SEEK_CUR );
  if( aftermem1_offset == -1 )  return -1;
  
  ans = igf_findmem(
    fd, mem2, mem2size, buff, buffsize );

  if( ans == -1 )  return -1;
  // Reset the fd offset since we moved after
  // finding mem1
  if( ans == 0 )  {

    if( lseek( fd, keepoffset, SEEK_SET ) == -1 )
      return -1;
    return 0;

  }

  // this means we found the mem1 as well as mem2
  // Now jus set how many bytes wee have between them
  const off_t mem2_offset = lseek( fd, 0, SEEK_CUR );
  if( mem2_offset == -1 )  return -1;

  *bytesnum = ( size_t )( mem2_offset - aftermem1_offset );

  // Now move back to offset just after mem1 and return

  if( lseek( fd, aftermem1_offset, SEEK_SET ) == -1 )
    return -1;

  return 1;

}

