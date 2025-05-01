// 	cone.h
//	definitions for a cone
//
//	created: 	1994-02-17 J Rydin
//	revision:   A

#ifndef __CONE_H__
#define __CONE_H__

#include "defs.h"
#include "prim2.h"
#include "line.h"
#include "trans.h"

class CCone : public CPrimitive {

public: // Constructor

	CCone(CLine C, double R, CTexture* T, CPrimitive* B, int F);
	CCone(vector Base, vector Apex, double R, CTexture* T, CPrimitive* B, int F);


public: // Services

	virtual Intersection Intersect(CRay& Ray) const;
	virtual vector Normal(const vector&) const;
	virtual int Inside(const vector& Point, const CPrimitive*) const;

	virtual void Translate(vector T);
	virtual void Scale(vector S);
	virtual void Rotate(vector R);

//	virtual void Serialize(CArchive A);

private:// First impl. only real. a cylinder.

	CLine  m_pl;
	double m_Radius;

	//	These are variables based on m_pl an m_Radius that are used
	//	in calculations:

	double t_Radius_2;
	double t_dl;
                              
	CTrans m_Transform;                              
                              
};

inline
int CCone::Inside(const vector& Point, const CPrimitive*) const {

	double a=m_pl.Dir()*(Point-m_pl.Loc());

	vector b = Point-a*m_pl.Dir()-m_pl.Loc();
	double k = b.NormOf();

	return (k<=m_Radius)^m_FlipInside;

}

#endif
