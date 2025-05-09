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

    // flipInside: invert inside test if true
    CUnion(bool flipInside, CPrimitive* B) noexcept : CGroup(flipInside, B) {}


public: // Services

	virtual Intersection Intersect(CRay& Ray) const;
    // Returns true if point is inside union (with optional flip)
    virtual bool	Inside(const vector& a, const CPrimitive* C) const noexcept;

//	virtual void Serialize(CArchive A);

};

inline
bool CUnion::Inside(const vector& a, const CPrimitive* C) const noexcept {

    return CGroup::Inside(a, C) != (m_FlipInside != 0);

}

#endif
