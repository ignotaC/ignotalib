/*

Copyright (c) 2023 Piotr Trzpil  p.trzpil@protonmail.com

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

#include "igf_purge.h"
#include "igf_read.h"
#include "igf_opt.h"

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

// Reads bytes from fd to buff untill error on read
// EOF or buff is full.
// Returns read bytes size or 0 if EOF.
// -1 is returned on error ( read error ).
// Restarts on EINTR
//
// ON ERROR we might lend up with changed flags status
int igf_purge_tillblock(
  const int fd,
  void *const buff,
  size_t readsize
)  {

  assert( fd >= 0 );
  assert( buff != NULL );
  assert( readsize != 0 );

  // set  nonblocking 

  int flags;
  if( ( flags = igf_getflags( fd ) ) == -1 )
    return -1;

  if( igf_nonblock( fd ) == -1 )
    return -1;

  ssize_t  readret = 0;
  for(;;)  {

    readret = read( fd, buff, readsize );
    if( readret > 0 )  continue;
    if( readret == 0 )  break; // this is a bit rare case but can happen
    
    // now we have errors

    switch( errno )  {

      case EAGAIN:
      #if ( EAGAIN != EWOULDBLOCK )
      case EWOULDBLOCK:
      #endif
        break;
      default:
	return -1;

    }

  }

  // We have purged data , set back the flags and  finish
  if( igf_setflags( fd, flags ) == -1 )
    return -1;
 
  return 0;
  
}



