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

int ign_inet4tcpcli( 
    const char *const ip,
    const unsigned short port
)  {

  assert( ip != NULL );

  // set up sockaddr struct
  struct sockaddr_in sin4 = {0}; // this must be zeroed
  sin4.sin_family = AF_INET;
  sin4.sin_port = htons( port );
  int pton_ret = inet_pton( AF_INET, ip, &( sin4.sin_addr ) );
  if( pton_ret != 1 )  return -1; // 0 and -1 are fail here

  
  // set up socket
  int clifd = socket( AF_INET, SOCK_STREAM, 0 );
  if( clifd == -1 )  return -1;


  // TODO see the connect in manual behavior for EINTR asynch not precise
  // same socket or any with same struct ?
  // connect to server
  if( connect( clifd, ( struct sockaddr* )&sin4 , sizeof( sin4 ) ) == -1 )
    goto error_cleanup;
  
  return clifd;
  
error_cleanup:
  close( clifd );
  return -1;

}


// IP6
int ign_inet6tcpcli( 
    const char *const ip,
    const unsigned short port
)  {

  assert( ip != NULL );

  // set up sockaddr struct
  struct sockaddr_in6 sin6 = {0}; // this must be zeroed
  sin6.sin6_family = AF_INET6;
  sin6.sin6_port = htons( port );
  int pton_ret = inet_pton( AF_INET6, ip, &( sin6.sin6_addr ) );
  if( pton_ret != 1 )  return -1; // 0 and -1 are fail here

  
  // set up socket
  int clifd = socket( AF_INET6, SOCK_STREAM, 0 );
  if( clifd == -1 )  return -1;


  // TODO see the connect in manual behavior for EINTR asynch not precise
  // same socket or any with same struct ?
  // connect to server
  if( connect( clifd, ( struct sockaddr* )&sin6 , sizeof( sin6 ) ) == -1 )
    goto error_cleanup;
  
  return clifd;
  
error_cleanup:
  close( clifd );
  return -1;

}


// I assume we prefer v6
int ign_inettcpcli(
    const char *const ip,
    const unsigned short port
)  {

  assert( ip != NULL );

  errno = 0; // must do this since we can fail on inet_pton
	     // also beware what can happen if on AF_INET we pass ipv6 str
	     // so this order is safer - first ipv6
  int clifd = ign_inet6tcpcli( ip, port );
  if( clifd == -1 )  {

    // if errno == 0 this means inet_pton failed
    // so meaby ipv4 addr is correct ?
    if( errno != 0 )
      return -1;

  }

  return ign_inet4tcpcli( ip, port );

}
