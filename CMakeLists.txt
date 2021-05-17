cmake_minimum_required( VERSION 3.0 )

project( IgnotaLib VERSION 0.1.0 )

set( CMAKE_C_STANDARD 99 )
set( CMAKE_C_STANDARD_REQUIRED true )

file( GLOB IG_SOURCE "src/*.c" )

configure_file( src/ignota_conf.h.in src/ignota_conf.h )

include_directories( "src" )
