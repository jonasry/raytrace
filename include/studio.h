// studio.h: declaration of CStudio
#ifndef __STUDIO_H__
#define __STUDIO_H__

#include <vector>
#include "group.h"

class CLight;
class CTexture;

class CStudio {
public:
    // depth: maximum recursion depth for ray tracing
    CStudio(int depth = 5);
    ~CStudio();

    std::vector<CLight*> Lights;
    std::vector<CTexture*> Textures;
    CGroup Objects;
    int RecurseDepth;
};

#endif // __STUDIO_H__