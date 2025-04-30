//	blend.h
//
//	created: 	1994-05-01 J Rydin
//	revision:   A

#ifndef __BLEND_H__
#define __BLEND_H__

class CColor;

double Blend(double C1, double C2, double n, double a0, double a1, double a2, double a3);
CColor Blend(CColor C1, CColor C2, double n, double a0, double a1, double a2, double a3);

#endif
