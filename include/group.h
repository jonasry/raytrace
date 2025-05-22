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

    public:
        // B: bounding primitive, flipInside=false
        CGroup(CPrimitive* B) noexcept;
        virtual ~CGroup() override;
    protected:
        // B: bounding primitive; flipInside=true inverts inside test
        CGroup(bool flipInside, CPrimitive* B) noexcept;


public: // Services

	virtual Intersection Intersect(CRay& Ray) const override;
	virtual vector Normal(const vector& Point) const override;
	// Returns true if point is inside any member primitive (excluding C)
	virtual bool Inside(const vector& Point, const CPrimitive*) const noexcept override;
	
	virtual void Translate(CVector T) override;
	virtual void Scale(CVector S) override;
	virtual void Rotate(CVector R) override;

//	virtual void Serialize(CArchive A);


public: // Data
    // Container of child primitives
    std::vector<pCPrimitive> Objects;


};

inline
vector CGroup::Normal(const vector& Point) const {
    (void)Point;
    return CVector(0,0,0);
}

#endif
