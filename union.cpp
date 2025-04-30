#include "ray.h"
#include "union.h"

CRTBase::Intersection CUnion::Intersect(CRay& Ray) const {

	Intersection I;
	CRay L=Ray;


	do {

		I = CGroup::Intersect(L);

		if (I.IntersectedObject==0) return Intersection(0);

		vector a = L.PointAt(I.Distance);

		if (!(Inside(a,I.IntersectedObject)^m_FlipInside)) {
			I.Distance=(a-Ray.Loc()).NormOf();
			return I;
		}

		L.Move(I.Distance+m_eps);


	} while (I.IntersectedObject);

	return Intersection(0);

}
