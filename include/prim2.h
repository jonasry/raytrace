// 	prim.h
//	Primitive object class
//
//	created: 	1993-09-29 J Rydin
//	revision:   A

#ifndef __PRIM_H__
#define __PRIM_H__
#include <memory>

//#pragma message("PRIM.H")

#include "rtdefs.h"
#include "rtbase.h"		//	Base class	
#include "wfbase.h"		// 	Base class
#include "sceneob.h"	//	Base class

class CTexture;

class CPrimitive : public CRTBase, public CWFBase, public CSceneObject {

public:
	enum ClassId {Plane=1, Sphere=2, Cone=3, Group=4, Intersec=5, Union=6, Box=7, Strange=0};
    // flipInside: if true, inside/outside test is inverted


public:	// Constructors

    CPrimitive(CTexture* theTexture, bool flipInside, CPrimitive* theBound, ClassId C=Strange) noexcept;
    virtual ~CPrimitive() = default;


public: // Services

	int IntersectBound(CRay& Ray) const;
	CTexture* Texture() const;
        // Set flip flag: if true, inside/outside test is inverted
        virtual void SetInside(bool inside = false) noexcept;

	int Id() const { return m_Id; }	//	class identification

    virtual void Translate(vector T) { (void)T; }
    virtual void Rotate(vector R) { (void)R; }
    virtual void Scale(vector S) { (void)S; }

//	virtual void Serialize(CArchive A);

protected:

	static double 	m_eps; // A small value used in intersection routine
	ClassId 		m_Id;
    bool    		m_FlipInside;


private:// Data

    std::unique_ptr<CPrimitive> m_BoundingObject;
	CTexture*		m_TextureObject;
	

};

typedef CPrimitive* pCPrimitive;

// ===============================
// = Inline functions            =
// ===============================

inline
CTexture* CPrimitive::Texture() const {

	return m_TextureObject;

}

inline
int CPrimitive::IntersectBound(CRay& Ray) const {

	return m_BoundingObject==0 ?
		   1 : (m_BoundingObject->Intersect(Ray).IntersectedPrimitive!=0);

}

inline
void CPrimitive::SetInside(bool inside) noexcept {
    m_FlipInside = inside;
}

#endif
