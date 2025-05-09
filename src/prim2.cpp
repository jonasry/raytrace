#include "prim2.h"

double CPrimitive::m_eps = 1e-6;

CPrimitive::CPrimitive(CTexture* theTexture, bool flipInside, CPrimitive* theBound, ClassId C) noexcept
   : m_Id(C),
     m_FlipInside(flipInside),
     m_BoundingObject(theBound),
     m_TextureObject(theTexture) {}
