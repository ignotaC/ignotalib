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

#ifndef IGF_WRITE_H
#define IGF_WRITE_H

#include <sys/types.h>

ssize_t igf_write(
    const int fd,
    void *const buff,
    size_t writesize
);

ssize_t igf_writeall_nb(
    const int fd,
    void *const buff,
    size_t writesize,
    const long wait_milisec
);


#endif
