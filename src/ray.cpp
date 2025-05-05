//	ray.cpp
//
//	created: 	1993-09-21 J Rydin
//	revision:   A

#include "ray.h"
#include "light.h"
#include "studio.h"
#include "prim2.h"
#include "texture.h"  // for CTexture


int Shaded(CRay::DifferentialArea dA, const CStudio* S, const CLight* L);

CColor CRay::Sample(const CStudio* Studio, unsigned RecurseLevel) {


	CColor Intensity(0,0,0);


	// Get a sample for this ray
	//
	// 1. Search for intersection with objects
	// 2. Add illumination from lights
	// 3. Add illumination from light reflected from
	//	  other objects.
	// 4. Add illumination from light transmitted through
	//	  intersected object


	CRTBase::Intersection I = Studio->Objects.Intersect(*this);

	if (I.IntersectedPrimitive==0)		// 	The ray didn't intersect.
		return CColor(.2,.55,.85)*.5;  	//  Background color




	//	Replace the intersected object with a small differential plane:

	m_dA.Loc 	= PointAt(I.Distance);
	m_dA.Texture= I.IntersectedPrimitive->Texture();
	m_dA.Normal = I.IntersectedPrimitive->Normal(m_dA.Loc);

	if ((Dir()*m_dA.Normal)>0.0)
		m_dA.Normal = -m_dA.Normal;


	//	Add direct illumination:
    // Add direct illumination from each light
    for (auto* L : Studio->Lights) {
        if (!Shaded(m_dA, Studio, L)) {
            vector v = L->Loc() - m_dA.Loc;
            double a = exp(-v.NormOf() / 20.0); // Attenuation (TEMPORARY)
            Intensity += L->Illuminate(m_dA, *this) * a;
        }
    }



	if (RecurseLevel==Studio->RecurseDepth)	return Intensity;



	//	Add indirect illumination: (Temporary)

	if ( m_dA.Texture->Ks(m_dA.Loc) > .0 ) { // consider nonzero level

		vector	ReflectedDirection = Dir() - 2*(Dir() * m_dA.Normal)*m_dA.Normal;
		CRay*	R = new CRay( ReflectedDirection, m_dA.Loc );

		Intensity += R->Sample(Studio, RecurseLevel+1)
			* m_dA.Texture->Reflectivity(m_dA.Loc, m_dA.Normal, ReflectedDirection);

		delete R;

	}

	double a = exp(-I.Distance/20.);
	return Intensity*a + CColor(.2,.55,.85)*.5*(1-a);

}

int Shaded(CRay::DifferentialArea dA, const CStudio* S, const CLight* L) {

	if (L->NoShade()) return 0;

	vector v = L->Loc()-dA.Loc;

	CRay R(v,dA.Loc);

    CRTBase::Intersection I = S->Objects.Intersect(R);

	if (I.IntersectedPrimitive==0) return 0;

	double Dist = v.NormOf();
	if (I.Distance<Dist) return 1;

	return 0;

}
