//	Box.cpp

#include "box.h"
#include "ray.h"
#include "texture.h"

#define LEFT 0
#define MIDDLE 1
#define RIGHT 2

CBox::CBox(vector Max, vector Min, CTexture* T, CPrimitive* B, int F):
	CPrimitive(T,F,B,Box), m_maxB(Max), m_minB(Min){}


CRTBase::Intersection CBox::Intersect(CRay& Ray) const {
                       
	int i;
	int inside = 1;
	int quadrant[3];
	int whichPlane;
	double maxT[3];
	double candidatePlane[3];
	vector dir=Ray.Dir();
	vector loc=Ray.Loc();
	           
	m_Transform.invTransDirection(dir);	           
	m_Transform.invTransPoint(loc);	           
	           
	((CBox *)this)->m_Normal.Set(0,0,0);

	for (i=0; i<3; i++)
		if(loc[i] < m_minB[i]) {
			quadrant[i] = LEFT;
			candidatePlane[i] = m_minB[i];
			inside = 0;
		} else if (loc[i] > m_maxB[i]) {
			quadrant[i] = RIGHT;
			candidatePlane[i] = m_maxB[i];
			inside = FALSE;
		} else	{
			quadrant[i] = MIDDLE;
		}	



	for (i = 0; i < 3; i++)
		if (quadrant[i] != MIDDLE && dir[i] !=0.)
			maxT[i] = (candidatePlane[i]-loc[i]) / dir[i];
		else
			maxT[i] = -1.;



	whichPlane = 0;
	for (i = 1; i < 3; i++)
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;


	if (maxT[whichPlane] < 0.) 
		return Intersection(0);
		
	double coord[3]; 
		
	for (i = 0; i < 3; i++)
		if (whichPlane != i) {
			coord[i] = loc[i] + maxT[whichPlane] * dir[i];
/*			if ((quadrant[i] == RIGHT && coord[i] < m_minB[i]) ||
			    (quadrant[i] == LEFT  && coord[i] > m_maxB[i])) */

			if ((coord[i] < m_minB[i]) ||
			    (coord[i] > m_maxB[i]))
				return Intersection(0);	/* outside box */
		} else {
			coord[i] = candidatePlane[i];
			if (coord[i]==m_maxB[i])
				((CBox *)this)->m_Normal.Set(i,1);
			else
				((CBox *)this)->m_Normal.Set(i,-1);
		}
         
	CVector hit(coord);
	m_Transform.transPoint(hit);
         
	double t = (hit - Ray.Loc()).NormOf();

	return Intersection(t,this);
}
       

vector CBox::Normal(const vector& Point) const {

	// currently using cached normal, m_Normal, stored from last intersect
                                      
	CVector point(Point); m_Transform.invTransPoint(point);                                                                          
	CVector normal=Texture()->Normal(point,m_Normal);
	m_Transform.transDirection(normal);
	
	return normal.Normalize();

}


int CBox::Inside(const vector& Point, const CPrimitive*) const {

	int inside=1;
	CVector point(Point); 

	m_Transform.invTransPoint(point);

	for (int i=0; i<3; i++)
		if(Point[i] < m_minB[i]) 
			inside = 0;
	 	else if (Point[i] > m_maxB[i]) 
			inside = 0;
		
	return inside;

}



void CBox::Translate(vector T) {
 
	m_Transform.translate(T); 
	
	Texture()->Translate(T);

}


void CBox::Scale(vector S) {
 
	m_Transform.scale(S); 

	Texture()->Scale(S);

}


void CBox::Rotate(vector R) {
 
	m_Transform.rotate(R); 

	Texture()->Rotate(R);

}
