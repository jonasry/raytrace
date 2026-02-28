//	ray.cpp
//
//	created: 	1993-09-21 J Rydin
//	revision:   A

#include "ray.h"
#include "light.h"
#include "studio.h"
#include "prim2.h"
#include "texture.h"  // for CTexture
#include <algorithm>
#include <cmath>

int Shaded(CRay::DifferentialArea dA, const CStudio* S, const CLight* L);

namespace {

constexpr double kAirIOR = 1.0;
constexpr double kRayBias = 1e-4;

double Clamp(double value, double lo, double hi) {
	return std::max(lo, std::min(value, hi));
}

bool SupportsBasicRefraction(const CPrimitive* primitive) {
	if (primitive == nullptr) {
		return false;
	}

	return primitive->Id() != CPrimitive::Plane &&
	       primitive->Id() != CPrimitive::Group;
}

double FresnelReflectance(double cosThetaI, double etaI, double etaT) {
	cosThetaI = Clamp(cosThetaI, 0.0, 1.0);

	const double r0 = (etaI - etaT) / (etaI + etaT);
	const double base = r0 * r0;
	const double oneMinusCos = 1.0 - cosThetaI;

	return base + (1.0 - base) * oneMinusCos * oneMinusCos *
	       oneMinusCos * oneMinusCos * oneMinusCos;
}

bool RefractDirection(const vector& incident,
                      const vector& normal,
                      double etaI,
                      double etaT,
                      vector& refracted) {
	const double eta = etaI / etaT;
	const double cosThetaI = Clamp(-(incident * normal), 0.0, 1.0);
	const double sinThetaTSquared = eta * eta * (1.0 - cosThetaI * cosThetaI);

	if (sinThetaTSquared > 1.0) {
		return false;
	}

	const double cosThetaT = std::sqrt(1.0 - sinThetaTSquared);
	refracted = eta * incident + (eta * cosThetaI - cosThetaT) * normal;
	refracted = !refracted;
	return true;
}

} // namespace

CColor CRay::Sample(const CStudio* Studio, int RecurseLevel, double CurrentIOR) {
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
		return Studio->BackgroundColor;  	//  Background color

	//	Replace the intersected object with a small differential plane:

	m_dA.Loc = PointAt(I.Distance);
	m_dA.Texture = I.IntersectedPrimitive->Texture();

	vector SurfaceNormal = I.IntersectedPrimitive->Normal(m_dA.Loc);
	const bool FrontFace = (Dir() * SurfaceNormal) < 0.0;
	m_dA.Normal = FrontFace ? SurfaceNormal : -SurfaceNormal;

	// Add direct illumination:
    // Add direct illumination from each light
    for (const auto& lightPtr : Studio->Lights) {
        const CLight* L = lightPtr.get();
        if (!Shaded(m_dA, Studio, L)) {
            vector v = L->Loc() - m_dA.Loc;
            double a = exp(-v.NormOf() / 20.0); // Attenuation (TEMPORARY)
            Intensity += L->Illuminate(m_dA, *this) * a;
        }
    }

	const double Transparency = Clamp(m_dA.Texture->Transparency(m_dA.Loc), 0.0, 1.0);
	const bool UseRefraction =
		Transparency > 0.0 && SupportsBasicRefraction(I.IntersectedPrimitive);
	const double OpaqueWeight =
		UseRefraction ? Clamp(1.0 - Transparency, 0.0, 1.0) : 1.0;
	Intensity = Intensity * OpaqueWeight;

	if (RecurseLevel==Studio->RecurseDepth)	return Intensity;

	//	Add indirect illumination: (Temporary)

	if (OpaqueWeight > 0.0 && m_dA.Texture->Ks(m_dA.Loc) > .0) {
		vector	ReflectedDirection = Dir() - 2*(Dir() * m_dA.Normal)*m_dA.Normal;
		CRay R(ReflectedDirection, m_dA.Loc + m_dA.Normal * kRayBias);

		Intensity += R.Sample(Studio, RecurseLevel+1)
			* (m_dA.Texture->Reflectivity(m_dA.Loc, m_dA.Normal, ReflectedDirection)
			   * OpaqueWeight);
	}

	if (UseRefraction) {
		const double NextIOR = FrontFace ? m_dA.Texture->IOR(m_dA.Loc) : kAirIOR;
		const double CosThetaI = Clamp(-(Dir() * m_dA.Normal), 0.0, 1.0);
		double Reflectance = FresnelReflectance(CosThetaI, CurrentIOR, NextIOR);
		Reflectance = Clamp(Reflectance, 0.0, 1.0);

		vector ReflectedDirection = Dir() - 2*(Dir() * m_dA.Normal)*m_dA.Normal;
		CRay ReflectedRay(ReflectedDirection, m_dA.Loc + m_dA.Normal * kRayBias);

		vector RefractedDirection;
		if (RefractDirection(Dir(), m_dA.Normal, CurrentIOR, NextIOR, RefractedDirection)) {
			CRay RefractedRay(RefractedDirection, m_dA.Loc - m_dA.Normal * kRayBias);
			Intensity += ReflectedRay.Sample(Studio, RecurseLevel + 1, CurrentIOR) *
			             (Reflectance * Transparency);
			Intensity += RefractedRay.Sample(Studio, RecurseLevel + 1, NextIOR) *
			             ((1.0 - Reflectance) * Transparency);
		} else {
			Intensity += ReflectedRay.Sample(Studio, RecurseLevel + 1, CurrentIOR) *
			             Transparency;
		}
	}

	double a = exp(-I.Distance/20.);
	return Intensity * a + Studio->BackgroundColor * (1 - a);
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
