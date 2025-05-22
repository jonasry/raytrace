// 	box.h
//	definitions for a box.
//
//	created: 	1995-07-07 J Rydin
//	revision:   A

#ifndef __BOX_H__
#define __BOX_H__

#include "prim2.h"
#include "texture.h"
#include "trans.h"

class CBox : public CPrimitive {

public: // Constructor

    // flipInside: invert inside test if true
    CBox(const vector& Max, const vector& Min, CTexture* T,
         CPrimitive* B, bool flipInside) noexcept;


public: // Services

	virtual Intersection Intersect(CRay& Ray) const override;
	virtual vector Normal(const vector&) const override;
    virtual bool Inside(const vector& Point, const CPrimitive*) const noexcept override;

	virtual void Translate(vector T) override;
	virtual void Scale(vector S) override;
	virtual void Rotate(vector R) override;

//	virtual void Serialize(CArchive A);

protected:// Data

	vector m_Normal, m_maxB, m_minB;
	
	CTrans m_Transform;

};

#endif
