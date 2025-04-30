// studio.h: declaration of CStudio
#ifndef __STUDIO_H__
#define __STUDIO_H__

#include "list2.h"
#include "group.h"

class CLight;
class CTexture;

class CStudio {
public:
    // depth: maximum recursion depth for ray tracing
    CStudio(int depth = 5);
    ~CStudio();

    CObList Lights;
    CObList Textures;
    CGroup Objects;
    int RecurseDepth;
};

#endif // __STUDIO_H__