// 	turb.h
//	turbulence texture class for raytracing applications.
//
//	created: 	1994-05-01 J Rydin
//	revision:   A

#ifndef __TURB_H__
#define __TURB_H__

#include "noise.h"
#include "blend.h"

class CTurbulence : public CNoise {

public:	// 	Constructor

	CTurbulence(CTexture* Tx1, CTexture* Tx2, double c=1) : CNoise(Tx1,Tx2,c), m_Limit(0.01),
															a0(0), a1(2), a2(0), a3(-1) {}

public:	//  Services:

	virtual vector Normal(const vector& P, const vector& N) const;

	virtual CColor Diffuse(const vector &) const;
	virtual CColor Specular(const vector &) const;
	virtual CColor Reflectivity(const vector &, const vector &, const vector &);
	virtual double ShinePow(const vector &) const;
	virtual double Ks(const vector &) const;
	virtual double IOR(const vector &);

//	virtual void Serialize(CArchive A);


protected:

	double Turbulence(const vector& r) const;


protected:// Data:

	double m_Limit;
	
	double a3,a2,a1,a0;


};

inline
vector CTurbulence::Normal(const vector&, const vector& N) const {
	return N;
}

inline
CColor CTurbulence::Diffuse(const vector& r) const {
	return Blend(T1->Diffuse(r), T2->Diffuse(r), Turbulence(r/m_Scale),a3,a2,a1,a0) ;
}

inline
CColor CTurbulence::Specular(const vector& r) const {
	return Blend(T1->Specular(r), T2->Specular(r), Turbulence(r/m_Scale),a3,a2,a1,a0) ;
}

inline
CColor CTurbulence::Reflectivity(const vector& r, const vector & n, const vector & d) {
	return Blend(T1->Reflectivity(r,n,d), T2->Reflectivity(r,n,d), Turbulence(r/m_Scale),a3,a2,a1,a0) ;
}

inline
double CTurbulence::ShinePow(const vector& r) const {
	return Blend(T1->ShinePow(r), T2->ShinePow(r), Turbulence(r/m_Scale),a3,a2,a1,a0) ;
}

inline
double CTurbulence::Ks(const vector& r) const {
	return Blend(T1->Ks(r), T2->Ks(r), Turbulence(r/m_Scale),a3,a2,a1,a0) ;
}

inline
double CTurbulence::IOR(const vector& r) {
	return Blend(T1->IOR(r), T2->IOR(r), Turbulence(r/m_Scale),a3,a2,a1,a0) ;
}

#endif
