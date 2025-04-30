//	checker.cpp

#include "checker.h"
#include "trans.h"

const CTexture* CChecker::SelectTexture(const vector& P) const {

	int x = int(floor(P*m_X/m_Scale_x));
	int y = int(floor(P*m_Y/m_Scale_y));
	int z = int(floor(P*m_Z/m_Scale_z));

	if ( ((x+y+z)%2)==0 )
		return m_Even;

	else
		return m_Odd;

}
                                
void CChecker::Translate(vector T) {

	m_Transform.translate(T);

}                                

void CChecker::Scale(vector S) {

	m_Transform.scale(S);

	m_Scale_x = S.GetX();
	m_Scale_y = S.GetY();
	m_Scale_z = S.GetZ();
	
}

void CChecker::Rotate(vector R) {

	m_Transform.rotate(R);
	
	m_Transform.transDirection(m_X);
	m_Transform.transDirection(m_Y);
	m_Transform.transDirection(m_Z);

}
