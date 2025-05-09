//	rtbase.h

#ifndef __RTBASE_H__
#define __RTBASE_H__
                               
#include "vector.h"

class CPrimitive;
class CRay;

//	CRTBase is a base class for raytrace specific methods and subclasses.
//	No data is stored in a CRTBase object. It is purely virtual.

class CRTBase {

public: // constructor

	CRTBase(){}
	virtual ~CRTBase() = default;

public: //Structures

	struct Intersection {	//	Used temporarely to store data about
							//	an intersection..

		//	Data members:

		double Distance;
		const  CPrimitive* IntersectedObject;
		const  CPrimitive* IntersectedPrimitive;

		//	Constructors:

		Intersection()
			: 	Distance(100000000.0),IntersectedPrimitive(0),
				IntersectedObject(0){}
		Intersection(double D, const CPrimitive* P=0):
			Distance(D), IntersectedPrimitive(P), IntersectedObject(0) {}

		//	Operator overloading

        bool operator==(const CPrimitive* P) const noexcept { return IntersectedPrimitive == P; }
        bool operator!=(const CPrimitive* P) const noexcept { return IntersectedPrimitive != P; }

	};

public: // Raytrace-specific methods

	virtual Intersection Intersect(CRay& Ray) const = 0;
    // Returns true if the point is inside the primitive
    virtual bool Inside(const vector&, const CPrimitive*) const noexcept = 0;
	virtual	vector Normal(const vector&) const = 0;

};

#endif
