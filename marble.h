// 	marble.h
//	Marble texture class for raytracing applications.
//
//	created: 	1994-05-01 J Rydin
//	revision:   A

#ifndef __MARBLE_H__
#define __MARBLE_H__

#include "turb.h"
#include "blend.h"

class CMarble : public CTurbulence {

public:	// 	Constructor

	CMarble(CTexture* Tx1, CTexture* Tx2, double c=1) : CTurbulence(Tx1,Tx2,c),
														a0(0), a1(0.5), a2(0), a3(0.5) {}


public:	//  Services:

	virtual vector Normal(const vector& P, const vector& N) const;

	virtual CColor Diffuse(const vector &) const;
	virtual CColor Specular(const vector &) const;
	virtual CColor Reflectivity(const vector &, const vector &, const vector &);
	virtual double ShinePow(const vector &) const;
	virtual double Ks(const vector &) const;
	virtual double IOR(const vector &);

//	virtual void Serialize(CArchive A);


private:

	double Marble(const vector& r) const;
	

private: // Data:

	double a3,a2,a1,a0;	// coeffs for blending function


};

inline
vector CMarble::Normal(const vector&, const vector& N) const {
	return N;
}

inline
CColor CMarble::Diffuse(const vector& r) const {
	return Blend(T1->Diffuse(r), T2->Diffuse(r), Marble(r/m_Scale),a3,a2,a1,a0) ;
}

inline
CColor CMarble::Specular(const vector& r) const {
	return Blend(T1->Specular(r), T2->Specular(r), Marble(r/m_Scale),a3,a2,a1,a0) ;
}

inline
CColor CMarble::Reflectivity(const vector& r, const vector & n, const vector & d) {
	return Blend(T1->Reflectivity(r,n,d), T2->Reflectivity(r,n,d), Marble(r/m_Scale),a3,a2,a1,a0) ;
}

inline
double CMarble::ShinePow(const vector& r) const {
	return Blend(T1->ShinePow(r), T2->ShinePow(r), Marble(r/m_Scale),a3,a2,a1,a0) ;
}

inline
double CMarble::Ks(const vector& r) const {
	return Blend(T1->Ks(r), T2->Ks(r), Marble(r/m_Scale),a3,a2,a1,a0) ;
}

inline
double CMarble::IOR(const vector& r) {
	return Blend(T1->IOR(r), T2->IOR(r), Marble(r/m_Scale),a3,a2,a1,a0) ;
}

#endif
