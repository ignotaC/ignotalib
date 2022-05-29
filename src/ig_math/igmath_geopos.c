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

#include "igmath_geopos.h"

#include <stdio.h>

#include <assert.h>
#include <math.h>

int igmath_get_geopos(
    struct igmath_geopos *const gp,
    const double latitude, // deg
    const char latitude_hemisphere, // N or S
    const double longtitude, // deg
    const char longtitude_hemisphere // W or E
)  {

  // a bit broken way to check by better than nothing 
  assert( ( latitude < 90.00001 ) ||
    ( latitude > -0.00001 ) );
  assert( ( longtitude < 180.00001 ) ||
    ( longtitude > -0.00001 ) );
  assert( ( latitude_hemisphere == 'N' ) ||
    ( latitude_hemisphere == 'S' ) );
  assert( ( longtitude_hemisphere == 'W' ) ||
    ( longtitude_hemisphere == 'E' ) );

  // convert degries to radians
  double lat_in_radians = latitude * IGMATH_PI / 180.0;
  double lon_in_radians = longtitude * IGMATH_PI / 180.0;
 
  // first let us use the ellipse that by rotating creates the
  // elipsoid model of earth
  // ( r / Re ) ^2 + ( Z / Rp )^2 = 1
  

  double latitude_tg = tan( lat_in_radians );

  // count r
  double latitude_radious =
     ( 1.0 / IGMATH_EQUATORIAL_RADIUS ) * 
     ( 1.0 / IGMATH_EQUATORIAL_RADIUS );
  latitude_radious +=
    ( latitude_tg / IGMATH_POLAR_RADIUS ) *
    ( latitude_tg / IGMATH_POLAR_RADIUS );
  latitude_radious = 1.0 / latitude_radious;  
  // at this point latitude_radious is r^2
  // we need to sqare it later but to count Z it's
  // better to use it instead of taking power of it later

  // Now we can count Z position
  double Z_pos =
    ( IGMATH_EQUATORIAL_RADIUS ) * IGMATH_EQUATORIAL_RADIUS;
  Z_pos = 1.0 - ( latitude_radious / Z_pos );
  Z_pos *= 
    ( IGMATH_POLAR_RADIUS ) * IGMATH_POLAR_RADIUS;
  Z_pos = sqrt( Z_pos );
  if( latitude_hemisphere == 'S' )
    Z_pos = -Z_pos;

  // finish counting the latitude radious
  latitude_radious = sqrt( latitude_radious );

  // We are left now with getting X and Y
  // but the *hardest job* was done already
  double X_pos = latitude_radious * sin( lon_in_radians );
  if( longtitude_hemisphere == 'W' )
    X_pos = -X_pos;

  // here cosinus simply sets Y negative or positive
  double Y_pos = latitude_radious * cos( lon_in_radians );

  // as this point we have everythign for a 3d position
  gp->x = X_pos;
  gp->y = Y_pos;
  gp->z = Z_pos;

  return 0;

}


