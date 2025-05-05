// studio.h: declaration of CStudio
#ifndef __STUDIO_H__
#define __STUDIO_H__

#include <vector>
#include <memory>
#include "group.h"

class CLight;
class CTexture;

class CStudio {
public:
    // depth: maximum recursion depth for ray tracing
    CStudio(int depth = 5);
    ~CStudio();

    std::vector<std::unique_ptr<CLight>> Lights;
    std::vector<std::unique_ptr<CTexture>> Textures;
    CGroup Objects;
    int RecurseDepth;
};

#endif // __STUDIO_H__