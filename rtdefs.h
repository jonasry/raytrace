#ifndef __RTDEFS_H__
#define __RTDEFS_H__

#ifndef TRUE
const int TRUE = 1;
#endif

#ifndef FALSE
const int FALSE = 0;
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

typedef unsigned short scoord;

#include "color.h"

inline double sqr(double a) { return a*a; }
inline CColor sqr(CColor a) { return a*a; }
          
#ifndef min
inline double min(double a, double b) { return a<b? a:b; }
#endif
#ifndef max
inline double max(double a, double b) { return a>b? a:b; }
#endif

#endif
