// 	optic.h
//	optic base class
//
//	created: 	1993-09-21 J Rydin
//	revision:   A

#ifndef __OPTIC_H__
#define __OPTIC_H__

#include "rtdefs.h"         
#include "sceneob.h"

class CLine;

class COptic : public CSceneObject {

public: // Constructor

	COptic(const COptic& O);
	COptic(CLine ViewPoint, vector UP, double h_angle, double v_angle);


public: //  Services

	void SetLocation(const vector& NewLOC) { LOC=NewLOC; }
	void SetDirection(const vector& NewDirection);
	void SetUpVector(const vector& NewUP);
	void SetHorizAngle(double NewHoriz);
	void SetVertAngle(double NewVert);
	
	vector GetLocation() { return LOC; }
	vector GetDirection() { return Y; }
	vector GetUpVector() { return m_UP; }
	double GetHorizAngle() { return m_horiz; }
	double GetVertAngle() {  return m_vert; }

	virtual void Translate(vector T) {}
	virtual void Rotate(vector R) {}
	virtual void Scale(vector S) {}

//	virtual void Serialize(CArchive A);

public:	//	Public data
	//	Vectors needed to define the position and viewing direction
	//	of the camera.

	vector LOC;		//	Location of the system
	vector X,Y,Z;	//	Orientation of system (unit vectors)
	vector U,V;		//	Orientation and size of screen

private: //  Private data
	
	//	Save these values for peeking.

	vector m_UP;
	double m_horiz, m_vert;


};


#endif
