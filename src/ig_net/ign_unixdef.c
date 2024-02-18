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

#include <sys/socket.h>

#include <assert.h>
#include <string.h>

// it won't set to zeros sun it onlu checks if name fits in
// and then copies it in, finaly sets family
int ign_unixstruct_set(
    struct sockaddr_un *const sun,
    const char *const unixname
)  {

  assert( sun != NULL );
  assert( unixname != NULL );

  size_t sunname_maxlen = sizeof sun->sun_path;

  size_t namelen = strnlen( unixname, sunname_maxlen );
  // If no nul or too long = than this is error
  if( namelen == sunname_maxlen )  return -1;

  // namelen + 1 since we are sure now there is place for nul
  strncpy( sun->sun_path, unixname, namelen + 1 );
  sun->sun_family = AF_UNIX;  // posix family 
  return 0; 

}

