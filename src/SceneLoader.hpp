// SceneLoader.hpp
// Parses a YAML scene file and populates the CStudio and camera list.
#pragma once

#include <string>
#include <vector>
#include <memory>

#include "studio.h"
#include "camera.h"

namespace ryml { struct Tree; }

class SceneLoader {
public:
    // Load scene from YAML file into provided studio and cameras vector.
    // Returns true on success, false on error.
    static bool load(const std::string& filename,
                     CStudio& studio,
                     std::vector<std::unique_ptr<CCamera>>& cameras);
};