#include "prim2.h"

double CPrimitive::m_eps = 1e-6;

CPrimitive::CPrimitive(CTexture* theTexture, int FlipN, CPrimitive* theBound, ClassId C)
	:m_TextureObject(theTexture), m_FlipInside(FlipN),
	 m_BoundingObject(theBound), m_Id(C) {}
