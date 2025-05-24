// Studio.cpp

#include "studio.h"
#include "light.h"
#include "texture.h"

// Constructor initializes lists and recursion depth
CStudio::CStudio(int depth)
    : Lights()
    , Textures()
    , Objects(0)
    , BackgroundColor(CColor(0,0,0))
    , RecurseDepth(depth)
    , Oversampling(1)
{
}

// Destructor deletes allocated lights and textures; scene objects are managed by CGroup
CStudio::~CStudio()
{
    // no-op: unique_ptr in Lights and Textures will handle destruction
}
