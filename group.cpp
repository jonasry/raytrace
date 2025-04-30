#include "group.h"

//	Constructor:

CGroup::CGroup(int F,CPrimitive* B) : CPrimitive(0,F,B,Group) {}
CGroup::CGroup(CPrimitive* B) : CPrimitive(0,0,B,Group) {}

CGroup::~CGroup() {
 
	POSITION T1, T2;
	pCPrimitive obj;
	T1 = Objects.GetHeadPosition();
    
    while (T1!=0) {
    	T2 = T1;
		obj = (pCPrimitive) Objects.GetNext(T1);
		Objects.RemoveAt(T2);
		delete obj;
	}
}    

CRTBase::Intersection CGroup::Intersect(CRay& Ray) const {

	Intersection I;
	Intersection Nearest;
	CPrimitive* Obj;

	//	Check intersection agains bounding object:

	if (!IntersectBound(Ray)) return Intersection(0);

	//	Ok - Now check out intersection with all members of group
	//	and return the nearest intersection.

	POSITION T = Objects.GetHeadPosition();

	while (T!=0) { 
	
        Obj=(CPrimitive *)Objects.GetNext(T);
        
		I = Obj->Intersect(Ray);

		if (I.IntersectedPrimitive!=0)
			if ((I.Distance<Nearest.Distance)&&(I.Distance>m_eps)) {
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

	pCPrimitive Obj;

	POSITION T = Objects.GetHeadPosition();

	while (T!=0) {                      
	
		Obj=(pCPrimitive)Objects.GetNext(T);
		
		if (Obj==C) continue;
		if(Obj->Inside(Point,0)) return TRUE;
	}

	return FALSE;

}
                                 

void CGroup::Translate(vector T) {
 
	pCPrimitive Obj;

	POSITION P = Objects.GetHeadPosition();

	while (P!=0) {                      
	
		Obj=(pCPrimitive)Objects.GetNext(P);
		
		Obj->Translate(T);

	}

}


void CGroup::Scale(vector S) {
 
	pCPrimitive Obj;

	POSITION P = Objects.GetHeadPosition();

	while (P!=0) {                      
	
		Obj=(pCPrimitive)Objects.GetNext(P);
		
		Obj->Scale(S);

	}

}


void CGroup::Rotate(vector R) {
 
	pCPrimitive Obj;

	POSITION P = Objects.GetHeadPosition();

	while (P!=0) {                      
	
		Obj=(pCPrimitive)Objects.GetNext(P);
		
		Obj->Rotate(R);

	}

}
