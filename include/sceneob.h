// 	sceneob.h
//	Pure virtual base class for object appearing in the raytracing scene
//
//	created: 	1994-05-15 J Rydin
//	revision:   A

#ifndef __SCENEOB_H__
#define __SCENEOB_H__  

#include "object.h"
#include "vector.h"
       
class CSceneObject : public CObject {
                  
public:
	CSceneObject(){}
	virtual ~CSceneObject() = default;
	
public:
	//	Transformations of 
	virtual void Translate(vector) = 0;
	virtual void Rotate(vector)= 0;
	virtual void Scale(vector) = 0;
	
//	virtual void Serialize(CArchive A);

};
       
#endif 
