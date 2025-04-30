//	Interfac.cpp

#include "interfac.h"

CInterface::~CInterface() {

	POSITION T1, T2;
	CObject* obj;

	T1 = Cameras.GetHeadPosition();
    
    while (T1!=NULL) {
    	T2 	= T1;
		obj = Cameras.GetNext(T1);
		Cameras.RemoveAt(T2);
		delete (CCamera *) obj;
	}

}

void CInterface::SnapAll() {

	CCamera* Cam;
	POSITION C = Cameras.GetHeadPosition();

	while (C!=NULL) {
		Cam=(CCamera *) Cameras.GetNext(C);
		Cam->Snap();
	}

}
