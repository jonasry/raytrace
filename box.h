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

	CBox(vector Max, vector Min, CTexture* T, CPrimitive* B, int F);


public: // Services

	virtual Intersection Intersect(CRay& Ray) const;
	virtual vector Normal(const vector&) const;
	virtual int Inside(const vector& Point, const CPrimitive*) const;

	virtual void Translate(vector T);
	virtual void Scale(vector S);
	virtual void Rotate(vector R);

//	virtual void Serialize(CArchive A);

protected:// Data

	vector m_Normal, m_maxB, m_minB;
	
	CTrans m_Transform;

};

#endif
