#include "group.h"

//	Constructor:

// Constructor: flipInside determines inversion of inside test
CGroup::CGroup(bool flipInside, CPrimitive* B) noexcept
    : CPrimitive(nullptr, flipInside, B, Group) {}
// Default constructor: no inversion
CGroup::CGroup(CPrimitive* B) noexcept
    : CPrimitive(nullptr, false, B, Group) {}

CGroup::~CGroup() {
    // Delete all child primitives and clear the container
    for (auto* obj : Objects) {
        delete obj;
    }
    Objects.clear();
}

CRTBase::Intersection CGroup::Intersect(CRay& Ray) const {

	Intersection I;
	Intersection Nearest;

	//	Check intersection agains bounding object:

	if (!IntersectBound(Ray)) return Intersection(0);

    // Check intersection with all members of the group and return the nearest
    for (auto* Obj : Objects) {
        I = Obj->Intersect(Ray);
        if (I.IntersectedPrimitive != nullptr && I.Distance < Nearest.Distance && I.Distance > m_eps) {
            Nearest = I;
            Nearest.IntersectedObject = Obj;
        }
    }

	return Nearest;


}

bool CGroup::Inside(const vector& Point, const CPrimitive* C) const noexcept {

	//	Point is inside the group if Point is inside one or more of
	//	the members of the group.

	//	Object C is a member of the group, and Point belongs to its
	//	surface.

	//	A Group in it self can not flip inside with outside.


    // Check if any member contains the point (excluding C)
    for (auto* Obj : Objects) {
        if (Obj == C) continue;
        if (Obj->Inside(Point, nullptr)) return true;
    }
    return false;

}
                                 

void CGroup::Translate(vector T) {
    // Translate all child primitives
    for (auto* Obj : Objects) {
        Obj->Translate(T);
    }
}


void CGroup::Scale(vector S) {
    // Scale all child primitives
    for (auto* Obj : Objects) {
        Obj->Scale(S);
    }
}


void CGroup::Rotate(vector R) {
    // Rotate all child primitives
    for (auto* Obj : Objects) {
        Obj->Rotate(R);
    }
}
