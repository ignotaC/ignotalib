

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

#include <assert.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

// Function fills igds_chrarr struct with strings.
// Each string is name of file in the directory.
// This function on success will clear errno value
// It can fail on realloc, malloc inside chrarr_add
// and in opendir, readdir.
// Closedir is not checked for error.
// Returns -1 on fail and 0 on success.
// On failure the opened dirname is closed but saved
// names to igds_chrarr are not touched. So it end
// up partialy filled.
int igf_getdirfiles(
    const char *const dirname,
    struct igds_strarr *const filenames
)  {

  assert( dirname != NULL );
  assert( filenames != NULL );

  DIR *dir = opendir( dirname );
  if( dir == NULL ) return -1;

  size_t namelen = 0;
  char *newname = NULL;
  char **keep_filenames = NULL;
  for( struct dirent *dp = NULL ;;)  {

    errno = 0;
    dp = readdir( dir );
    if( dp == NULL )  {

      if( errno != 0 )  goto readdir_err;
      closedir( dir );
      return 0;

    }

    namelen = strnlen( dp->d_name, NAME_MAX );
    keep_filenames = filenames->entry;
    if( igds_strarr_addent( filenames, dp->d_name,
       namesize ) == -1 )  goto addent_err;
    // nul is added inside strarr_addent

  }

 addent_err:
 readdir_err:
  int keep_err = errno;
  closedir( dir );
  errno = keep_err;
  return -1;

}

