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

    // flipInside: invert inside test if true
    CIntersection(bool flipInside, CPrimitive* B) noexcept : CGroup(flipInside, B) {}


public: // Services

	virtual Intersection Intersect(CRay& Ray) const;
    // Returns true if point is inside intersection (with optional flip)
    virtual bool Inside(const vector& a, const CPrimitive* C) const noexcept;

//	virtual void Serialize(CArchive A);

};

#endif
