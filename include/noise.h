// 	noise.h
//	noise texture class for raytracing applications.
//
//	created: 	1994-05-01 J Rydin
//	revision:   A

#ifndef __NOISE_H__
#define __NOISE_H__

#include "texture.h"
#include "blend.h"

class CNoise : public CTexture {

public:	// 	Constructor

	CNoise(CTexture* Tx1, CTexture* Tx2, double c=1) : T1(Tx1), T2(Tx2), m_Scale(c),
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

	double Noise(const vector& r) const;


protected: // Data:

	CTexture* T1;
	CTexture* T2;
	double m_Scale; 
	double a3,a2,a1,a0;

};

inline
vector CNoise::Normal(const vector&, const vector& N) const {
	return N;
}

inline
CColor CNoise::Diffuse(const vector& r) const {
	return Blend(T1->Diffuse(r), T2->Diffuse(r), Noise(r/m_Scale),a3,a2,a1,a0) ;
}

inline
CColor CNoise::Specular(const vector& r) const {
	return Blend(T1->Specular(r), T2->Specular(r), Noise(r/m_Scale),a3,a2,a1,a0) ;
}

inline
CColor CNoise::Reflectivity(const vector& r, const vector & n, const vector & d) {
	return Blend(T1->Reflectivity(r,n,d), T2->Reflectivity(r,n,d), Noise(r/m_Scale),a3,a2,a1,a0) ;
}

inline
double CNoise::ShinePow(const vector& r) const {
	return Blend(T1->ShinePow(r), T2->ShinePow(r), Noise(r/m_Scale),a3,a2,a1,a0) ;
}

inline
double CNoise::Ks(const vector& r) const {
	return Blend(T1->Ks(r), T2->Ks(r), Noise(r/m_Scale),a3,a2,a1,a0) ;
}

inline
double CNoise::IOR(const vector& r) {
	return Blend(T1->IOR(r), T2->IOR(r), Noise(r/m_Scale),a3,a2,a1,a0) ;
}

#endif
