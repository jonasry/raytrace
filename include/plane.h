// 	plane.h
//	definitions for a plane.
//
//	created: 	1993-09-23 J Rydin
//	revision:   A

#ifndef __PLANE_H__
#define __PLANE_H__

#include "prim2.h"
#include "texture.h"
#include "trans.h"

class CPlane : public CPrimitive {

public: // Constructor

    // flipInside: if true, inside test is inverted
    CPlane(const vector& C, const vector& N, CTexture* T,
           CPrimitive* B, bool flipInside) noexcept;


public: // Services

	virtual Intersection Intersect(CRay& Ray) const;
	virtual vector Normal(const vector&) const;
	// Returns true if the point is inside the plane half-space (may be inverted)
	virtual bool Inside(const vector& Point, const CPrimitive*) const noexcept;

	virtual void Translate(vector T);
	virtual void Scale(vector S);
	virtual void Rotate(vector R);

//	virtual void Serialize(CArchive A);

protected:// Data

	double m_Center;	//	The perpendicular distance to origo
	vector m_Normal;

	CTrans m_Transform;

};

#endif
