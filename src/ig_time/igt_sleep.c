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

#include "igt_sleep.h"

#include <assert.h>
#include <errno.h>

// Function uses nanosleep
// You pass time in miliseconds you want program to sleep
// MAXIMUM time should be less than one second. And bigger/equal zero.
// EINTR is handled
int igt_sleepmilisec(
    const long milisec_time
)  {

  assert( milisec_time >= 0 );
  assert( milisec_time < 1000 );

  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = milisec_time * 1000000; // make  miliseconds

  for(;;)  {

    if( nanosleep( &ts, &ts ) == 0 )  return 0;
    if( errno != EINTR )  return -1;

  }

}

// Function uses nanosleep
// You pass time in seconds and microseconds you want program to sleep
// EINTR is handled
int igt_sleep(
    const time_t sec_time,
    const long mikrosec_time
)  {

  assert( mikrosec_time >= 0 );
  assert( mikrosec_time < 1000000 );

  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = mikrosec_time * 1000; // make  microseconds

  for(;;)  {

    if( nanosleep( &ts, &ts ) == 0 )  return 0;
    if( errno != EINTR )  return -1;

  }

}

