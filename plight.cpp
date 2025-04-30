#include "plight.h"
#include "texture.h"

CColor Shine(const vector&, const vector&, const CRay::DifferentialArea&);


//	Constructor(s)

CPointLight::CPointLight(const vector& L, const CColor& I):
	CLight(I), m_Center(L) {}


//	Services:

CColor CPointLight::Illuminate(const CRay::DifferentialArea& dA, const CRay& V) const {

	vector L = !(m_Center - dA.Loc);

	double alfa = L*dA.Normal;

	if (alfa <= 0.0) return CColor(0,0,0);

	double a = dA.Texture->Ks(dA.Loc);

	CColor Intensity(0,0,0);

	if (a<1.0)
		Intensity = alfa*dA.Texture->Diffuse(dA.Loc)*(1-a);

	if (a>0.0)
		Intensity+= Shine(L,-V.Dir(),dA)*a;

	return Intensity*m_Intensity;

}
