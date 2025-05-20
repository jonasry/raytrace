// 	sphere.h
//	definitions for a sphere
//
//	created: 	1993-09-21 J Rydin
//	revision:   A

#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "prim2.h"
#include "texture.h"
#include "trans.h"

class CSphere : public CPrimitive {

public: // Constructor

    // flipInside: invert inside test if true
    CSphere(double R, const vector& C, CTexture* T,
            CPrimitive* B, bool flipInside) noexcept;


public: // Services

	virtual Intersection Intersect(CRay& Ray) const;
	virtual vector Normal(const vector& Point) const override;
    virtual bool Inside(const vector& Point, const CPrimitive*) const noexcept;
	
	virtual void Translate(vector T);
	virtual void Scale(vector S);
	virtual void Rotate(vector R);

//	virtual void Serialize(CArchive A);

private:

	double m_Radius;
	vector m_Center;
	
	CTrans m_Transform;


};

// Removed inline implementation of CSphere::Normal
// inline
// vector CSphere::Normal(const vector& Point) const {
// 
// 	return Texture()->Normal(Point,!(Point-m_Center));
// 
// }

inline
bool CSphere::Inside(const vector& Point, const CPrimitive*) const noexcept {

	double a = (Point-m_Center).NormOf();

    bool inside = (a < m_Radius);
    return inside != m_FlipInside;

}

#endif
