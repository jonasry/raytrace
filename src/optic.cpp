//	optic.cpp

#include "optic.h"
#include "line.h"

COptic::COptic(CLine ViewPoint, vector UP, double h_angle, double v_angle) :
	LOC(ViewPoint.Loc()), Y(ViewPoint.Dir()) {

	//	Set up an orthogornal system of unitvectors:

	X = CrossProduct(Y,!UP);
	Z = CrossProduct(X,Y);

	//	Set up the screen vectors (screen is perpendicular to view-dir):

	U = tan(h_angle/180.0*M_PI) * X;
	V =-tan(v_angle/180.0*M_PI) * Z;

}

COptic::COptic(const COptic& O) :
	LOC(O.LOC), X(O.X), Y(O.Y), Z(O.Z), U(O.U), V(O.V) {}
	
void COptic::SetDirection(const vector& NewDirection) {
 
 	Y = !NewDirection;
	X = CrossProduct(Y,!m_UP);
	Z = CrossProduct(X,Y);
 	
 
}
	
void COptic::SetUpVector(const vector& NewUP) {

	X = CrossProduct(Y,!NewUP);
	Z = CrossProduct(X,Y);
	m_UP=!NewUP;

}                         

void COptic::SetHorizAngle(double NewHoriz) {

	U = tan(NewHoriz/180.0*M_PI) * X;
	m_horiz=NewHoriz;

}

void COptic::SetVertAngle(double NewVert) {

	V = -tan(NewVert/180.0*M_PI) * Z;
	m_vert=NewVert;

}
