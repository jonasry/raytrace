//	alight.h
//	Ambient lightsource


#ifndef ALIGHT_H
#define ALIGHT_H

#include "light.h"
#include "texture.h"

class CAmbientLight : public CLight {        

public: // Constructor

	CAmbientLight(const CColor& I):CLight(I){}


public: // Services

	virtual CColor Illuminate(const CRay::DifferentialArea& dA, const CRay& V) const;
	
	virtual void Translate(vector T) {}
	virtual void Rotate(vector R) {}
	virtual void Scale(vector S) {}

//	virtual void Serialize(CArchive A) { CLight::Serialize(); }
};

inline
CColor CAmbientLight::Illuminate(const CRay::DifferentialArea& dA, const CRay&) const {
	return m_Intensity*dA.Texture->Diffuse(dA.Loc)*(1-dA.Texture->Ks(dA.Loc));
}

#endif
