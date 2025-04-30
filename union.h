// 	union.h
//	definitions for a union of some objects.
//
//	created: 	1994-03-03 J Rydin
//	revision:   A

#ifndef __UNION_H__
#define __UNION_H__

#include "group.h"

class CUnion : public CGroup {

public: // Constructor

	CUnion(int F, CPrimitive* B) : CGroup(F,B) {}


public: // Services

	virtual Intersection Intersect(CRay& Ray) const;
	virtual int	Inside(const vector& a, const CPrimitive* C) const;

//	virtual void Serialize(CArchive A);

};

inline
int CUnion::Inside(const vector& a, const CPrimitive* C) const {

	return CGroup::Inside(a,C)^m_FlipInside;

}

#endif
