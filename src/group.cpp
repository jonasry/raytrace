#include "group.h"

//	Constructor:

CGroup::CGroup(int F,CPrimitive* B) : CPrimitive(0,F,B,Group) {}
CGroup::CGroup(CPrimitive* B) : CPrimitive(0,0,B,Group) {}

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

int CGroup::Inside(const vector& Point, const CPrimitive* C) const {

	//	Point is inside the group if Point is inside one or more of
	//	the members of the group.

	//	Object C is a member of the group, and Point belongs to its
	//	surface.

	//	A Group in it self can not flip inside with outside.


    // Check if any member contains the point (excluding C)
    for (auto* Obj : Objects) {
        if (Obj == C) continue;
        if (Obj->Inside(Point, nullptr)) return TRUE;
    }

	return FALSE;

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
