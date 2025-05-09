// SceneLoader.cpp
// Implements scene loading from YAML using RapidYAML

#include "SceneLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// RapidYAML includes (header-only)
#include <ryml.hpp>
#include <c4/yml/parse.hpp>
#include <c4/yml/access.hpp>

bool SceneLoader::load(const std::string& filename,
                       CStudio& studio,
                       std::vector<std::unique_ptr<CCamera>>& cameras) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Error: could not open scene file: " << filename << std::endl;
        return false;
    }
    std::ostringstream oss;
    oss << ifs.rdbuf();
    std::string data = oss.str();

    // Parse YAML in-place
    ryml::Tree tree = ryml::parse_in_place(ryml::to_csubstr(data));
    auto root = tree.rootref();

    // Example: load recursion_depth
    if (root.has_child("globals")) {
        auto gl = root["globals"];
        if (gl.has_child("recursion_depth")) {
            studio.RecurseDepth = gl["recursion_depth"].val().to_int();
        }
    }

    // TODO: implement parsing of output, camera, textures, lights, objects

    return true;
}