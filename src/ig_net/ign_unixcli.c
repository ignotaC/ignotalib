/*

Copyright (c) 2024 Piotr Trzpil  p.trzpil@protonmail.com

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

#include "ign_undef.h"
#include "ign_unixcli.h"

#include <sys/socket.h>
#include <sys/un.h>

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

// TODO - think about socketoptions
// TODO UDP and TCP

// beware unix socket if queue on listen is full will dropout with error immediatly
// TO DO - TIMEOUT VERSIONS

// TODO - MAKE VERSION WITH BIND ALSO

// REMEMBER - NO RELATIVE PATHS IN POSIX FOR SUN
// You pass string that is file name we want to open
// socket under it
// also can fail on socket function, bind
// -1 on Fail
// It tries to cleans socket on fail so no open
// sockfd  on success.
int ign_unixcli_strm_anon( 
    const char *const servname
)  {

  // assert check
  assert( servname != NULL );

  struct sockaddr_un sun = { 0 }; // this is very important
  size_t namelen = strnlen( servname, IGN_SUNNAME_MAXSIZE );
  // throw error on name with no nul
  if( namelen == IGN_SUNNAME_MAXSIZE )  {

    errno = 0;
    return -1;

  }

  strncpy( sun.sun_path, servname, namelen );
  sun.sun_family = AF_UNIX;

  // socket setup
  int sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
  if( sockfd == -1 )  return -1;

  // connect, handle eintr and we are ready to use it
  for(;;)  {

    if( connect( sockfd, ( struct sockaddr* ) &sun,
        sizeof( sun ) ) == -1 )  {

      if( errno == EINTR )  continue;
      goto sockfail;

    }

    break;

  }

  return sockfd;

  sockfail:
  close( sockfd );
  return -1;

}

int ign_unixcli_strm( 
    const char *const servname,
    const char *const cliname
)  {

  // assert check
  assert( servname != NULL );
  assert( cliname != NULL );

  // server struct ////
  struct sockaddr_un sun = { 0 }; // this is very important
  size_t namelen = strnlen( servname, IGN_SUNNAME_MAXSIZE );
  // throw error on name with no nul
  if( namelen == IGN_SUNNAME_MAXSIZE )  {

    errno = 0;
    return -1;

  }

  strncpy( sun.sun_path, servname, namelen );
  sun.sun_family = AF_UNIX;

  // client struct ////
  struct sockaddr_un cun = { 0 }; // this is very important
  namelen = strnlen( cliname, IGN_SUNNAME_MAXSIZE );
  // throw error on name with no nul
  if( namelen == IGN_SUNNAME_MAXSIZE )  {

    errno = 0;
    return -1;

  }

  strncpy( cun.sun_path, cliname, namelen );
  cun.sun_family = AF_UNIX;


  // socket setup
  int sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
  if( sockfd == -1 )  return -1;

  // connect, handle eintr and we are ready to use it
  for(;;)  {

    if( connect( sockfd, ( struct sockaddr* ) &sun,
        sizeof( sun ) ) == -1 )  {

      if( errno == EINTR )  continue;
      goto sockfail;

    }

    break;

  }

  return sockfd;

  sockfail:
  close( sockfd );
  return -1;

}

