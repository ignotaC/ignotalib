/*

Copyright (c) 2022 Piotr Trzpil  p.trzpil@protonmail.com

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

#include "ign_inetserv.h"

#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/socket.h>

#include <assert.h>
#include <errno.h>
#include <unistd.h>

// TODO set socetkoptions 
// some need to be set before server start other 
//  after meaby - I need clarification
// TODO let user set addr and also sockaddr since sometiems we need chaneg it

int ign_inet4tcpserv( 
    const unsigned short port,
    const int listen_queue
)  {

  assert( listen_queue != 0 );

  int servfd = socket( AF_INET, SOCK_STREAM, 0 );
  if( servfd == -1 )  return -1;

  // zero the tail is a must
  struct sockaddr_in sin4 = { 0 };
  sin4.sin_port = htons( port );
  sin4.sin_addr.s_addr = htonl( INADDR_ANY );
  sin4.sin_family = AF_INET; // ip4
  
  if( bind( servfd, ( struct sockaddr* )&sin4, 
      sizeof( sin4 ) ) == -1 )
    goto error_cleanup;

  if( listen( servfd, listen_queue ) == -1 )
    goto error_cleanup;

  return servfd;

error_cleanup:
  close( servfd );
  return -1;

}


// IP6
int ign_inet6tcpserv( 
    const unsigned short port,
    const int listen_queue
)  {

  assert( listen_queue != 0 );

  int servfd = socket( AF_INET6, SOCK_STREAM, 0 );
  if( servfd == -1 )  return -1;

  // zero the tail is a must
  struct sockaddr_in6 sin6 = { 0 };
  sin6.sin6_port = htons( port );
  sin6.sin6_addr= in6addr_any; 
  sin6.sin6_family = AF_INET6;
  
  if( bind( servfd, ( struct sockaddr* )&sin6, 
      sizeof( sin6 ) ) == -1 )
    goto error_cleanup;

  if( listen( servfd, listen_queue ) == -1 )
    goto error_cleanup;

  return servfd;

error_cleanup:
  close( servfd );
  return -1;

}


// I assume we prefer v6
int ign_inettcpserv(
    const unsigned short port,
    const int listen_queue
)  {

  assert( listen_queue != 0 );

  int servfd = ign_inet6tcpserv( port, listen_queue );
  if( servfd == -1 )  {

    if( errno != EAFNOSUPPORT )
      return -1;

  }

  return ign_inet4tcpserv( port, listen_queue );

}
