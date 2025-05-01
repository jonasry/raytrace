#include "ray.h"
#include "intersec.h"

CRTBase::Intersection CIntersection::Intersect(CRay& Ray) const {


	Intersection I;
	CRay L=Ray;


	do {

		I = CGroup::Intersect(L);

		if (I.IntersectedObject==0) return Intersection(0);

		vector a = L.PointAt(I.Distance);

		if (Inside(a,I.IntersectedObject)^m_FlipInside) {
			I.Distance=(a-Ray.Loc()).NormOf();
			return I;
		}

		L.Move(I.Distance+m_eps);


	} while (I.IntersectedObject);

	return Intersection(0);

}

int CIntersection::Inside(const vector& Point, const CPrimitive* C) const {

	//	Point is inside the intersection if Point is inside all of
	//	the members of the intersection

	//	Object C is a member of the intersection, and Point belongs to its
	//	surface. This object is discarded during the test, because it is
	//	not clear if Point belongs to C or not.

	CPrimitive* Obj;

	POSITION T = Objects.GetHeadPosition();

    while (T != Objects.GetEndPosition()) {
		Obj=(pCPrimitive) Objects.GetNext(T);
		if (Obj==C) continue;
		if (!Obj->Inside(Point,0))
			return FALSE^m_FlipInside;
	}

	return TRUE^m_FlipInside;


}
