// 	intersec.h
//	definitions for a intersection of some objects.
//
//	created: 	1994-03-02 J Rydin
//	revision:   A

#ifndef __INTERSEC_H__
#define __INTERSEC_H__

#include "group.h"

class CIntersection : public CGroup {

public: // Constructor

	CIntersection(int F,CPrimitive* B) : CGroup(F,B) {}


public: // Services

	virtual Intersection Intersect(CRay& Ray) const;
	virtual int Inside(const vector& a, const CPrimitive* C) const;

//	virtual void Serialize(CArchive A);

};

#endif
