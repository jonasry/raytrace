//	blend.cpp

#include "color.h"
#include "rtdefs.h"

double max_n=0, min_n=0;

CColor Blend(CColor C1, CColor C2, double n, double a0, double a1, double a2, double a3) {

	//n = n + 0.3;
	n=(n<-1.0)?-1:((n>1)?1:n);
	
	double c = (((a3*n+a2)*n+a1)*n+a0);
    double d = 1 - c;

	return C1*d + C2*c;

}

double Blend(double C1, double C2, double n, double a0, double a1, double a2, double a3) {

	//n = n + 0.3;
	n=(n<-1.0)?-1:((n>1)?1:n);

	double c = (((a3*n+a2)*n+a1)*n+a0);
    double d = 1 - c;

	return C1*d + C2*c;

}
