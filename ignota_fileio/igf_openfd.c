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


// Returns fd to read.
// It can fail on open function
// Tested, satisfied.
int igf_openfd_rd(
    const char *const filename )  {

  assert( filename != NULL );

  return   open( filename, O_RDONLY );
  
}


// Returns fd in appending mode.
// If no file to append than creates it
// Can fail on open function
int igf_openfd_ap(
    const char *const filename )  {

  assert( filename != NULL );
 
  return   open( filename, O_WRONLY | O_APPEND
    | O_CREAT, IGF_DEFAULT_FDMODE );

}


// Returns fd in writing mode.
// If file exist it will be removed - overwritten
// Can fail on open function
int igf_openfd_wr(
    const char *const filename )  {

  assert( filename != NULL );

  return   open( filename, O_WRONLY | O_TRUNC
    | O_CREAT, IGF_DEFAULT_FDMODE );

}


