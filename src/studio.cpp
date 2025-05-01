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
    CSceneObject* obj;
    while ((obj = Lights.RemoveTail()) != nullptr) {
        delete static_cast<CLight*>(obj);
    }
    // Delete all textures
    while ((obj = Textures.RemoveTail()) != nullptr) {
        delete static_cast<CTexture*>(obj);
    }
}
