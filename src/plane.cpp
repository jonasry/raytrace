//	Plane.cpp

#include "plane.h"
#include "ray.h"

CPlane::CPlane(vector C, vector N, CTexture* T, CPrimitive* B, int F): CPrimitive(T,F,B,Plane) {
	
	CVector normal(N), center(C);
	
	m_Transform.transDirection(normal);
	m_Transform.transPoint(center);
	
	m_Normal = !normal;
	m_Center = center*m_Normal;

}


CRTBase::Intersection CPlane::Intersect(CRay& Ray) const {

	// Temporary impl of transformation:

//	CRay Ray(R.Dir(), R.Loc()-translation);
	                
	// End temporary

	double Q = Ray.Dir() * m_Normal;

	if (fabs(Q)<m_eps)
		return Intersection(0);// Ray allmost parallel to plane

	double t = (m_Center - Ray.Loc() * m_Normal) / Q;

	if (t<m_eps) return Intersection(0);

	return Intersection(t,this);

}

vector CPlane::Normal(const vector& P) const {

	return Texture()->Normal(P,m_Normal);

}

int CPlane::Inside(const vector& P, const CPrimitive*) const {

	return ((P*m_Normal-m_Center)<=0)^m_FlipInside;

}

void CPlane::Translate(vector T) {
 	
 	m_Transform.translate(T);
 	
 	CVector C=m_Normal*m_Center;
 	m_Transform.transPoint(C);
 	
	m_Center=C*m_Normal;

	Texture()->Translate(T);

}

void CPlane::Scale(vector S) {
 	
 	m_Transform.scale(S);
 	
 	CVector C=m_Normal*m_Center;
 	m_Transform.transPoint(C);
 	m_Transform.transDirection(m_Normal);
 	
	m_Center=C*m_Normal;
	
	Texture()->Scale(S);

}

void CPlane::Rotate(vector R) {
 	
 	m_Transform.rotate(R);
 	
 	CVector C=m_Normal*m_Center;
 	m_Transform.transPoint(C);
 	m_Transform.transDirection(m_Normal);
 	
	m_Center=C*m_Normal;

	Texture()->Rotate(R);
}
