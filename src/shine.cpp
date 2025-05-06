#include "light.h"
#include "texture.h"

CColor Shine(const vector& L, const vector& V, const CRay::DifferentialArea& dA) {

	//	Temporary for implementation of Cook-Torrence illumination ekvation

	//--------------------------------------------------------//
	//                                                        //
	//  N = surfacenormal                                     //
	//  L = unitvector pointing from intersection to light    //
	//  V = unitvector parallell to the incoming ray          //
	//  H = halfway vector (unitvector of L+V)                //
	//                .                                       //
	//               L  N!    .V                              //
	//                 . !   .                                //
	//                   !  .                                 //
	//                  .! .                                  //
	//                   !.                                   //
	//  -----------------o------<Differential Area, dA>----   //
	//                   point                                //
	//                                                        //
	//  F = Fresnel value                                     //
	//  D = value of Beckman distrubution function            //
	//  G = geometric attunationfactor                        //
	//                                                        //
	//--------------------------------------------------------//

	vector H = Normalize(L+V);

	double nh= dA.Normal*H;
	double nv= dA.Normal*V;
	double nl= dA.Normal*L;
	double vh= V*H;

	double m = sqr(dA.Texture->ShinePow(dA.Loc));
	double d = -(1.0-sqr(nh))/(m*sqr(nh));

	if (d<-25.0) return CColor(0,0,0); // Critical value=>D too small

	double D = exp(d)/(4.0*m*pow(nh,4.0));
	if (D<.001) return CColor(0,0,0);

	double G = min( 1.0, 2.0 * nh/vh * min(nv, nl) );

	double c = L*H;

	CColor g = sqrt(sqr(dA.Texture->Specular(dA.Loc)) + sqr(c) - 1.0);
	CColor F = 0.5*(sqr((g-c)/(g+c))*(1.0+sqr((c*(g+c)-1.0)/(c*(g-c)+1.0))));

	return F*D*G/(1.0/(M_PI*nv*nl));


}
