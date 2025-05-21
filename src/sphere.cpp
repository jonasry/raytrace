//	sphere.cpp

#include "sphere.h"
#include "ray.h"
#include "trans.h"

CSphere::CSphere(double R, const vector& C, CTexture* T, CPrimitive* B, bool flipInside) noexcept
    : CPrimitive(T, flipInside, B, Sphere), m_Radius(R), m_Center(C) {}


CRTBase::Intersection CSphere::Intersect(CRay& Ray) const {

	//	Intersection ray - sphere

	vector a = Ray.Loc() - m_Center;
	double b = a*a;
	double c = a*Ray.Dir();
	double d = b - m_Radius*m_Radius;
	double e = c*c - d;

	if (e<0)
		return Intersection(0);	// Ray does not intersect with object

	double f=sqrt(e);
	double dist;

	if ( (dist=-c-f) > m_eps )
		return Intersection(dist,this);


	if ( (dist=-c+f) > m_eps )
		return Intersection(dist,this);


	return Intersection(0);

}


void CSphere::Translate(vector T) { 

 	m_Transform.translate(T);
	m_Center = m_Center + T; 
	
	Texture()->Translate(T);
	
}

void CSphere::Scale(vector S) { 
                         
	if ((S.GetX()!=S.GetY())||
		(S.GetY()!=S.GetZ())||
		(S.GetZ()!=S.GetX())) return; // Attempt to scale a sphere unevenly
                         
 	m_Transform.scale(S);

 	m_Transform.transPoint(m_Center);
	m_Radius = m_Radius*S.GetX();

	Texture()->Scale(S);
	
}

void CSphere::Rotate(vector R) {

 	m_Transform.rotate(R);
 	m_Transform.transPoint(m_Center);
 	
    Texture()->Rotate(R);
    
}

vector CSphere::Normal(const vector& Point) const {
    // Calculate the geometric normal for the sphere
    vector geometric_normal = Point - m_Center;
    geometric_normal.Normalize(); // Or: geometric_normal = !geometric_normal;

    // Delegate to the texture to allow modulation (e.g., bump mapping)
    // Ensure Texture() method is available and returns a valid CTexture*
    if (Texture()) { // Add a check for null texture pointer
        return Texture()->Normal(Point, geometric_normal);
    } else {
        // Fallback if no texture or texture doesn't modulate
        return geometric_normal; 
    }
}

