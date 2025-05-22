//	plight.h
//	pointlight lightsource


#ifndef __PLIGHT_H__
#define __PLIGHT_H__

#include "light.h"

class CPointLight : public CLight {

public: // Constructor

	CPointLight(const vector& L, const CColor& I);


public: // Services

	virtual CColor 	Illuminate(const CRay::DifferentialArea& dA, const CRay& V) const override;
	virtual	vector 	Loc() const override {return m_Center;}
	virtual int     NoShade() const override { return FALSE; }
	
	virtual void Translate(vector T) override { (void)T; }
	virtual void Rotate(vector R)    override { (void)R; }
	virtual void Scale(vector S)     override { (void)S; }

//	virtual void Serialize(CArchive A);

protected:

	vector m_Center;	//	Location of this light


};

#endif
