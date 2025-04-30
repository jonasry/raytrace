//	Studio.cpp
   
#include "studio.h"   
   
CStudio::~CStudio() {

	POSITION T1, T2;
	CObject * obj;
	
	T1 = Lights.GetHeadPosition();
    
	 while (T1!=0) {
    	T2 	= T1;
		obj = Lights.GetNext(T1);
		Lights.RemoveAt(T2);
		delete (CSceneObject *) obj;
	}

	T1 = Textures.GetHeadPosition();
    
	 while (T1!=0) {
    	T2 	= T1;
		obj = Textures.GetNext(T1);
		Textures.RemoveAt(T2);
		delete (CSceneObject *) obj;
	}

}
