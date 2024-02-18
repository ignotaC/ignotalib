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

#include "ign_unixdef.h"
#include "ign_uncli.h"

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
// TODO - decriptions


// REMEMBER - NO RELATIVE PATHS IN POSIX FOR SUN
// You pass string that is file name we want to open
// socket under it
// also can fail on socket function, bind
// -1 on Fail
// It tries to cleans socket on fail so no open
// sockfd  on success.
int ign_uncli_strm( 
    const char *const servname
)  {

  // assert check
  assert( servname != NULL );

  // server struct
  struct sockaddr_un sun = { 0 }; // always init to zero
  if( ign_unixstruct_set( &sun, servname ) == -1 )  {

    errno = 0;
    return -1;

  }

  // socket setup
  int sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
  if( sockfd == -1 )  return -1;

  // connect, handle eintr and we are ready to use it
  for(;;)  {

    if( connect( sockfd, ( struct sockaddr* ) &sun,
        sizeof sun ) == -1 )  {

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

int ign_uncli_strmid( 
    const char *const servname,
    const char *const idname
)  {

  // assert check
  assert( servname != NULL );
  assert( idname != NULL );

  // server struct
  struct sockaddr_un sun = { 0 }; // always init to zero
  if( ign_unixstruct_set( &sun, servname ) == -1 )  {

    errno = 0;
    return -1;

  }

  // client id struct
  struct sockaddr_un idun = { 0 }; // always init to zero
  if( ign_unixstruct_set( &idun, idname ) == -1 )  {

    errno = 0;
    return -1;

  }

  // socket setup
  int sockfd = socket( AF_UNIX, SOCK_STREAM, 0 );
  if( sockfd == -1 )  return -1;

  // set out id so server knows who is sending
  if( bind( sockfd, ( struct sockaddr* )&idun,
      sizeof idun ) == -1 )
    goto sockfail;

  // connect, handle eintr and we are ready to use it
  for(;;)  {

    if( connect( sockfd, ( struct sockaddr* )&sun,
        sizeof sun ) == -1 )  {

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


int ign_uncli_drgm( 
    const char *const servname
)  {

  // assert check
  assert( servname != NULL );

  // server struct
  struct sockaddr_un sun = { 0 }; // always init to zero
  if( ign_unixstruct_set( &sun, servname ) == -1 )  {

    errno = 0;
    return -1;

  }

  // socket setup
  int sockfd = socket( AF_UNIX, SOCK_DGRAM, 0 );
  if( sockfd == -1 )  return -1;

  // connect, handle eintr and we are ready to use it
  // we connect so we won't need  each time on snd/rcv
  // pass server structs
  for(;;)  {

    if( connect( sockfd, ( struct sockaddr* )&sun,
        sizeof sun ) == -1 )  {

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

int ign_uncli_dgrmid( 
    const char *const servname,
    const char *const idname
)  {

  // assert check
  assert( servname != NULL );
  assert( idname != NULL );

  // server struct
  struct sockaddr_un sun = { 0 }; // always init to zero
  if( ign_unixstruct_set( &sun, servname ) == -1 )  {

    errno = 0;
    return -1;

  }

  // client id struct
  struct sockaddr_un idun = { 0 }; // always init to zero
  if( ign_unixstruct_set( &idun, idname ) == -1 )  {

    errno = 0;
    return -1;

  }

  // socket setup
  int sockfd = socket( AF_UNIX, SOCK_DGRAM, 0 );
  if( sockfd == -1 )  return -1;

  // set our id so server knows who is sending
  if( bind( sockfd, ( struct sockaddr* )&idun,
      sizeof idun ) == -1 )
    goto sockfail;

  // connect, handle eintr and we are ready to use it
  // we connect so we won't need  each time on snd/rcv
  // pass server structs
  for(;;)  {

    if( connect( sockfd, ( struct sockaddr* )&sun,
        sizeof sun ) == -1 )  {

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


