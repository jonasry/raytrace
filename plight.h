//	plight.h
//	pointlight lightsource


#ifndef __PLIGHT_H__
#define __PLIGHT_H__

#include "light.h"

class CPointLight : public CLight {

public: // Constructor

	CPointLight(const vector& L, const CColor& I);


public: // Services

	virtual CColor 	Illuminate(const CRay::DifferentialArea& dA, const CRay& V) const;
	virtual	vector 	Loc() const {return m_Center;}
	virtual int     NoShade() const { return FALSE; }
	
	virtual void Translate(vector T) {}
	virtual void Rotate(vector R) {}
	virtual void Scale(vector S) {}

//	virtual void Serialize(CArchive A);

protected:

	vector m_Center;	//	Location of this light


};

#endif
