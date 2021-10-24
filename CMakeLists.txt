cmake_minimum_required( VERSION 3.0 )

project( ignota VERSION 0.1.1 LANGUAGES C )

set( CMAKE_VERBOSE_MAKEFILE true )

set( CMAKE_C_STANDARD 99 )
set( CMAKE_C_STANDARD_REQUIRED true )

set( CMAKE_C_FLAGS " -Wall -Wextra -pedantic -D_POSIX_C_SOURCE=200809L" )



configure_file( src/ignota_conf.h.in src/ignota_conf.h )

include( "cmake_loadinclude" )
