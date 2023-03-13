

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

#include "igmisc_getans.h"

#include <assert.h>
#include <stdio.h>

// get answer from user, clear stdin before reading ans.
// So garbage data is discarded but you can't automate passing it
// Fails on printf askign question
// 0 is NO 1 is YES
// -1 i serror
int igmisc_get_yn_ans(
    const char *const  question
)  {

  assert( question != NULL );

  if( printf( "%s\nAnswer y/Y for yes, "
      "anything else is treated as no.\n", question  ) > 0 )
    return -1;

  if( dumpread( stdin ) == -1 )
    return -1;

  int ans = getc( stdin );
  switch( ans )  {

    case 'Y':
    case 'y':
      return 1;
    case EOF:
      if( ferror( stdin ) )  return -1;
    default:
      return 0;

  }


}

