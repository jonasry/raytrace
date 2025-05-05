// 	group.h
//	definitions for a group.
//
//	created: 	1993-09-29 J Rydin
//	revision:   A

#ifndef __GROUP_H__
#define __GROUP_H__

#include "prim2.h"
#include <vector>
#include <memory>

class CGroup : public CPrimitive {

// Constructor

public:		CGroup(CPrimitive* B);
			virtual ~CGroup();
protected:  CGroup(int F,CPrimitive* B);


public: // Services

	virtual Intersection Intersect(CRay& Ray) const;
	virtual vector Normal(const vector& Point) const;
	virtual int Inside(const vector& Point, const CPrimitive*) const;
	
	virtual void Translate(CVector T);
	virtual void Scale(CVector S);
	virtual void Rotate(CVector R);

//	virtual void Serialize(CArchive A);


public: // Data
    // Container of child primitives
    std::vector<pCPrimitive> Objects;


};

inline
vector CGroup::Normal(const vector& Point) const {

	return CVector(0,0,0);

}

#endif
