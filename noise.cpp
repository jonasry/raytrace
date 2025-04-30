//	noise.cpp
//	noise and turbulence functions

#include "marble.h"
#include "rtdefs.h"

double fnoise3(const vector &);
double *noise3(const vector &);

double CNoise::Noise(const vector& r) const {

//	double *d = noise3(r);
	
//	return (r.GetX()*d[0] + r.GetY()*d[1] + r.GetZ()*d[2] + d[3])/9;

	double x = r.GetX();
	double y = r.GetY();
	double z = r.GetZ();


	return 	(sin(1.1*x -  .7*y)*0.9
			+sin(1.7*y - 1.3*z)*0.7
			+sin(0.8*z + 1.5*x)*0.6
			+sin(0.3*x + 0.9*y)*0.8
			+sin( .7*y + 1.7*z)*0.7
			+sin( .7*z -  .5*x)*0.5
			)/4.3*2.0/1.5;

}
 
double CTurbulence::Turbulence(const vector& r) const {

	double t = 0;
	double s = 1;

	while (s > m_Limit) {
		t += fabs( s*Noise(r/s) );
		s /= 2;
	}

	return t/1.69; //	move to interval [-1,1] for correct blending

}
 
double CMarble::Marble(const vector& r) const {

	double a = sin (r.GetX() + 2*M_PI*Turbulence(r));
	return a;

}
 