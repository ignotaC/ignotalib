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



// ADD SORTING SO IT WONR BREAK ON ADDING NEW STUFF

#ifndef IGC_PROBABILITY_H
#define IGC_PROBABILITY_H

#include <stdlib.h>
#include <stdint.h>

// CHANGE NAMES TO SIMPLY WORD AND DICT
struct igc_word  {

  char *word;
  size_t wordlen;
  double probability;
  char *binary_expression;
  int binarylen;
  uint64_t count; // Maybe check if we not pass it

};


struct igc_dict  {

  size_t len;
  struct igc_word *wp;
  uint64_t count;

};

void igc_init_dict(
    struct igc_dict *const dp
);

void igc_init_word(
    struct igc_word *const wp
);

struct igc_word *igc_addword_dict(
    struct igc_dict *const dp,
    const char *const word,
    const size_t wordlen
);

int igc_recvword_dict(
    struct igc_dict *const dp,
    const char *const word,
    const size_t wordlen
);

void igc_free_dict(
    struct igc_dict *const dc
);

int igc_print_dict( 
    const struct igc_dict *const dp
);

int igc_print_word( 
    const struct igc_word *const wp
);

void igc_findprob_dict(
    struct igc_dict *const
);

#endif
