// 	checker.h
//	checker texture class for raytracing applications.
//
//	created: 	1993-10-xx J Rydin
//	revision:   A

#ifndef __CHECKER_H__
#define __CHECKER_H__

#include "texture.h"
#include "trans.h"

class CChecker : public CTexture {

public: // Constructor

	CChecker(CTexture* S1, CTexture* S2)
		: 	m_Odd(S1), m_Even(S2), m_Scale_x(1), m_Scale_y(1), m_Scale_z(1),
			m_X(1,0,0),m_Y(0,1,0),m_Z(0,0,1) {}

	CChecker(CTexture* S1, CTexture* S2, double x, double y, double z)
		: 	m_Odd(S1), m_Even(S2), m_Scale_x(x), m_Scale_y(y), m_Scale_z(z),
			m_X(1,0,0),m_Y(0,1,0),m_Z(0,0,1) {}

public: // Sevices

	virtual vector 	Normal(const vector& P, const vector& N) const;
	virtual CColor 	Diffuse(const vector & P) const;
	virtual CColor 	Specular(const vector &) const;
	virtual double 	ShinePow(const vector &) const;
	virtual double 	Ks(const vector &) const;
	
	virtual void    Translate(vector T);
	virtual void 	Scale(vector S);
	virtual void	Rotate(vector R);

//	virtual void Serialize(CArchive A);

protected:

	CTexture* m_Odd;
	CTexture* m_Even;
	double 	  m_Scale_x, m_Scale_y, m_Scale_z;
	vector	  m_X,m_Y,m_Z;


private:

	virtual const CTexture* SelectTexture(const vector& P) const;

	CTrans m_Transform;

};

inline
vector CChecker::Normal(const vector& P, const vector& N) const {
	return SelectTexture(P)->Normal(P,N);
}

inline
CColor CChecker::Diffuse(const vector & P) const {
	return SelectTexture(P)->Diffuse(P);
}

inline
CColor CChecker::Specular(const vector & P) const {
	return SelectTexture(P)->Specular(P);
}

inline
double CChecker::ShinePow(const vector & P) const {
	return SelectTexture(P)->ShinePow(P);
}

inline
double CChecker::Ks(const vector & P) const {
	return SelectTexture(P)->Ks(P);
}


#endif
