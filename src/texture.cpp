//	Texture.cpp
//	Implements basic features of class CTexture

#include "texture.h"
#include "rtdefs.h"

CColor CTexture::Reflectivity(const vector& Point,const vector& Normal,const vector& Dir) {

	double	gr, gg, gb;	//	Help variables for each rgb value
	double	nr, ng, nb;	//	Ior value for rgb
	double	Fr, Fg, Fb;	//	Fresnel coeffs in rgb

	double 	t = Dir*Normal; //	cosine of angle between the vectors

	CColor Spec = Specular(Point);

//	if (index of refraction of travelling media<=1) {
		nr = Spec.Red();
		ng = Spec.Green();
		nb = Spec.Blue();

/*	} else {
		nr = 1./Spec.Red();
		ng = 1./Spec.Green();
		nb = 1./Spec.Blue();
	}
*/
	double	t1;

	gr= ((t1=sqr(nr) + t*t - 1.)<0.) ? 0. : sqrt(t1);
	gg= ((t1=sqr(ng) + t*t - 1.)<0.) ? 0. : sqrt(t1);
	gb= ((t1=sqr(nb) + t*t - 1.)<0.) ? 0. : sqrt(t1);

	Fr=0.5*(sqr((gr-t)/(gr+t))*(1.+sqr((t*(gr+t)-1.)/(t*(gr-t)+1.))));
	Fg=0.5*(sqr((gg-t)/(gg+t))*(1.+sqr((t*(gg+t)-1.)/(t*(gg-t)+1.))));
	Fb=0.5*(sqr((gb-t)/(gb+t))*(1.+sqr((t*(gb+t)-1.)/(t*(gb-t)+1.))));

	return CColor(Fr, Fg, Fb) * Ks(Point);

}
