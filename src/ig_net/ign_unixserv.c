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

#include "ign_unixserv.h"

#include <sys/socket.h>
#include <sys/un.h>

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

// TODO - think about socketoptions
// TODO UDP and TCP

// You pass string that is file name we want to open
// socket under it
// it can fail if name is too big
// name string or fail
// also can fail on socket function, bind and listen
// -1 on Fail
// It cleans socket on fail so no open
// descriptors should be left on fail.
// 0 on success.
int ign_unixserv( 
    const char *const name,
    const int listen_queue
)  {

  // as for second assert not sure how
  // listen would react with queue passed 0
  assert( name != NULL );
  assert( listen_queue != 0 ); 

  struct sockaddr_un sun = { 0 }; // this is very important
  size_t namelen = strlen( name );
  // somehow I feel safer to force nul here at end
  if( namelen >= sizeof( sun.sun_path ) )  {

    errno = 0;
    return -1;

  }
  
  sun.sun_family = AF_UNIX;
  strcpy( sun.sun_path, name );

  int sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
  if( sockfd == -1 )  return -1;

  // Sizeof is safest thing, beware SUN_LEN
  if( bind( sockfd, ( struct sockaddr* ) &sun,
      sizeof( sun ) ) == -1 )
    goto sockfail;

  // Better to havebigger since if no place instant drop
  // connecting unix socket for you
  if( listen( sockfd, listen_queue ) == -1 )
    goto sockfail;

  return sockfd;

  sockfail:
  close( sockfd );
  return -1;

}


