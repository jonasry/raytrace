//	cone.cpp

#include "cone.h"
#include "ray.h" 
#include "texture.h"

CCone::CCone(CLine C, double R, CTexture* T, CPrimitive* B, int F)
	: CPrimitive(T,F,B,Cone), m_pl(C), m_Radius(R),
	  t_Radius_2(R*R), t_dl(m_pl.Dir()*m_pl.Loc()) {}


CCone::CCone(vector Base, vector Apex, double R, CTexture* T, CPrimitive* B, int F)
	: CPrimitive(T, F, B, Cone), m_pl(Apex-Base, Base), m_Radius(R),
	  t_Radius_2(R*R), t_dl(m_pl.Dir()*m_pl.Loc()) {}


CRTBase::Intersection CCone::Intersect(CRay& Ray) const {

	// Temporary impl of transformation:

//	CRay Ray(R.Dir(), R.Loc()-translation);
	                
	// End temporary

	vector r	= Ray.Dir();
	vector d	= m_pl.Dir();

	double t1	= 1 - sqr( (d*r) );

	if ( fabs (t1) < m_eps )
		return Intersection(0);


	vector k  = Ray.Loc();
	vector l  = m_pl.Loc();

	vector c  = k-l;
	double D  = c*c;

	double C  = d*k - t_dl;

	double t2 = ( r*c - ( d*r ) * C ) / t1;
	double t3 = ( sqr( C ) + t_Radius_2 - D ) / t1;

	double t4, t5, t6;

	if ( (t4 = sqr (t2) + t3)  < m_eps )
		return Intersection(0);

	double t7 = sqrt(t4);


	if ( (t5 =-t2 + t7) < m_eps )
		return Intersection(0);


	t6 = -t2 - t7;

	if ( t6 > m_eps )
		return Intersection(t6,this);

	return Intersection(t5,this);

}

vector CCone::Normal (const vector& Point) const {

	vector tmp = Point - m_pl.Loc();
	return !(tmp-(m_pl.Dir()*tmp)*m_pl.Dir());

}

        
void CCone::Translate(vector T) {
 
	m_Transform.translate(T);
	m_Transform.transLine(m_pl);
	
	t_dl=m_pl.Dir()*m_pl.Loc();
	
	Texture()->Translate(T);

}


void CCone::Scale(vector S) {
 
	m_Transform.scale(S); 
	m_Transform.transLine(m_pl);
	
	t_Radius_2*=S[0];

	Texture()->Scale(S);

}


void CCone::Rotate(vector R) {
 
	m_Transform.rotate(R); 
	m_Transform.transLine(m_pl);

	t_dl=m_pl.Dir()*m_pl.Loc();
	
	Texture()->Rotate(R);

}
