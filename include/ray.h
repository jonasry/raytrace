// 	ray.h
//	ray class for raytracing applications.
//
//	created: 	1993-09-21 J Rydin
//	revision:   A

#ifndef __RAY_H__
#define __RAY_H__

#include "defs.h"
#include "line.h"

class CRay : public CLine {

public: // Data

	// 	Differential area. Used when ray has intersected an object to
	//  replace that object in further calculations.

	struct DifferentialArea {

		vector Loc;
		vector Normal;
		CTexture* Texture;

	} m_dA;

public:	// Constructors

	CRay();
	CRay(const vector& dir, const vector& loc);
	CRay(const CLine& l);
	CRay(const vector& dir);

	~CRay();


public: // Services

	CColor Sample(const CStudio* Studio, unsigned RecurseLevel);


private:

//	CColor Value;	// Result of intersection with this ray

};

inline CRay::CRay() {}
inline CRay::CRay(const vector& dir, const vector& loc) : CLine(dir,loc) {}
inline CRay::CRay(const CLine& l) : CLine(l) {}
inline CRay::CRay(const vector& dir) : CLine(dir) {}

inline CRay::~CRay() {}

#endif
