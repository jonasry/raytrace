// 	texture.h
//	texture class for raytracing applications.
//
//	created: 	1993-09-30 J Rydin
//	revision:   A

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "vector.h"
#include "color.h"
#include "sceneob.h"

class CTexture : public CSceneObject {

public: // Constructor

	//	General constructor

	CTexture(CColor D,CColor Sh,double S,double Ks,double IOR=1) :
		m_Diffuse(D),m_Specular(Sh),m_ShinePow(S),m_Ks(Ks),m_IOR(IOR) {}

	//	Simplified constructor; assume diffuse only

	CTexture(CColor D) : m_Diffuse(D), m_Specular(0,0,0),
						 m_ShinePow(0), m_Ks(0), m_IOR(1) {}

protected:

	CTexture(){}


public: // Services

	virtual vector Normal(const vector& P, const vector& N) const;

	virtual CColor Diffuse(const vector &) const { return m_Diffuse; }
	virtual CColor Specular(const vector &) const { return m_Specular; }
	virtual CColor Reflectivity(const vector &, const vector &, const vector &);
	virtual double ShinePow(const vector &) const { return m_ShinePow; }
	virtual double Ks(const vector &) const { return m_Ks; }
	virtual double IOR(const vector &) { return m_IOR; }

	virtual void Translate(vector T) {}
	virtual void Rotate(vector R) {}
	virtual void Scale(vector S) {}

//	virtual void Serialize(CArchive A);

private:// Data

	CColor m_Diffuse;
	CColor m_Specular;
	double m_ShinePow;  
	double m_Ks;        //	Strength of reflectivity
	double m_IOR;		//	Index of refraction

};

inline
vector CTexture::Normal(const vector&, const vector& N) const {
	return N;
}                                      

#endif
