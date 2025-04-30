//	light.h


#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "defs.h"
#include "rtdefs.h"
#include "color.h"
#include "ray.h"                   
#include "sceneob.h"

class CLight : public CSceneObject {

public: // Constructors

	CLight (const CColor& I) :
		m_Intensity(I) {}

	virtual ~CLight() = default;

public: // Services

	virtual CColor 	Illuminate(const CRay::DifferentialArea& dA, const CRay& V) const=0;
	virtual CColor 	Intensity(const vector &) const { return m_Intensity; }
	virtual vector 	Loc() const { return CVector(0,0,0); }
	virtual int     NoShade() const { return TRUE; }

//	virtual void Serialize(CArchive A);

protected:// Data

	CColor m_Intensity;

};

#endif                  
