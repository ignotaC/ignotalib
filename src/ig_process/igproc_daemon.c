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

#include "igproc_daemon.h"

#include "../ig_event/igev_signals.h"

#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int igproc_daemon(
    bool setdirroot
)  {

  // make child and leave parentin case we are
  // session leader and the setsid will always work
  pid_t pid = 0;
  if( ( pid = fork() ) == -1 )
    return -1;

  // parent can go away _exit because we don't want to
  // run exit functons
  if( pid )  _exit( EXIT_SUCCESS );

  // at this point we can only log
  // errors or any other information
  
  // escape the process group 
  if( setsid() == -1 )
    return -1;

  // ignore terminal closing sending SGIHYP
  if( igev_sigign( SIGHUP ) == -1 )
    return -1;

  // now again do the fork trick so
  // the daemon can't get back to terminal
  // earlier it could with some dose of hacks - they say.
  // again same thing with _exit as earlier
  // do not run any exit functions just make parent leave.
  
  if( ( pid = fork() ) == -1 )
    return -1;
  if( pid )  _exit( EXIT_SUCCESS );

  // now set the working directory to root
  // if requested
  if( setdirroot )  {

    if( chdir( "/" ) == -1 )
      return -1;

  }

  // now let us clean up the stdio
  // also lets just ignore errors here.
  close( 0 );
  close( 1 ); 
  close( 2 ); // if anything went wrong it will 
	      // show up later on open

  // this brave assumption it will be opened in growing order.
  if( open( "\\dev\\null", O_RDWR ) == -1 )
    return -1;
  if( open( "\\dev\\null", O_RDWR ) == -1 )
    return -1;
  if( open( "\\dev\\null", O_RDWR ) == -1 )
    return -1;

  // Success we are daemonized
  return 0; 

}
