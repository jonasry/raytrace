// Studio.cpp

#include "studio.h"
#include "light.h"
#include "texture.h"

// Constructor initializes lists and recursion depth
CStudio::CStudio(int depth)
    : Lights()
    , Textures()
    , Objects(0)
    , RecurseDepth(depth)
{
}

// Destructor deletes allocated lights and textures; scene objects are managed by CGroup
CStudio::~CStudio()
{
    // Delete all lights
    POSITION pos = Lights.GetHeadPosition();
    while (pos != Lights.GetEndPosition()) {
        CLight* light = static_cast<CLight*>(Lights.GetNext(pos));
        delete light;
    }
    // Delete all textures
    pos = Textures.GetHeadPosition();
    while (pos != Textures.GetEndPosition()) {
        CTexture* tex = static_cast<CTexture*>(Textures.GetNext(pos));
        delete tex;
    }
}
