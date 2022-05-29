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

#ifndef IGMATH_GEOPOS_H
#define IGMATH_GEOPOS_H


// radius in meters for elipsoid model of earth
#define IGMATH_EQUATORIAL_RADIUS 6378140.0
#define IGMATH_POLAR_RADIUS 6356750.0
#define IGMATH_PI 3.141592653589
// TODO move PI elsewhere

// 3D position of point on earth
// using basic rotary elipsoid model
// ( X / Re )^2 + ( Y / Re )^2 + ( Z / Rp )^2 = 1
// Re - Equatorial radious
// Rp - Polar Radious

struct igmath_geopos {

  double x;
  double y;
  double z;

};

int igmath_get_geopos(
    struct igmath_geopos *const gp,
    const double latitude, // deg
    const char latitude_hemisphere, // N or S
    const double longtitude, // deg
    const char longtitude_hemisphere  // W or E
);

#endif
