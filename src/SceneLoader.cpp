// SceneLoader.cpp
// Implements scene loading from YAML using RapidYAML

#include "SceneLoader.hpp"
#include "testapp.h"      // for SetupStudio
#include "storage.h"      // for CStorage::ImgClass
#include "camera.h"
#include "line.h"
#include "optic.h"
#include "vector.h"
#include <c4/yml/parse.hpp>
#include <c4/yml/access.hpp>
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

    // 1. Globals
    if (root.has_child("globals")) {
        auto gl = root("globals");
        if (gl.has_child("recursion_depth")) {
            studio.RecurseDepth = gl("recursion_depth").val().to_int();
        }
    }

    // 2. Setup default scene (textures, lights, objects)
    SetupStudio(studio);

    // 3. Output parameters
    std::string out_fname = "output.png";
    int width = 200, height = 160;
    CStorage::ImgClass imgType = CStorage::PNG;
    if (root.has_child("output")) {
        auto out = root("output");
        if (out.has_child("filename")) {
            out_fname = out("filename").val().to_string();
        }
        if (out.has_child("format")) {
            auto fmt = out("format").val().to_string();
            if (fmt == "PNG") imgType = CStorage::PNG;
            else if (fmt == "JPG" || fmt == "JPEG") imgType = CStorage::JPG;
            else if (fmt == "TGA") imgType = CStorage::TGA;
            else if (fmt == "HDR") imgType = CStorage::HDR;
        }
        if (out.has_child("resolution")) {
            auto res = out("resolution");
            width  = res[0u].val().to_int();
            height = res[1u].val().to_int();
        }
    }

    // 4. Camera
    if (root.has_child("camera")) {
        auto cam = root("camera");
        // only perspective supported
        vector position(0,0,0), look_at(0,0,1), up(0,1,0);
        double fov_h = 40.0, fov_v = 40.0;
        if (cam.has_child("position")) {
            auto seq = cam("position");
            position = vector(
                seq[0u].val().to_double(),
                seq[1u].val().to_double(),
                seq[2u].val().to_double());
        }
        if (cam.has_child("look_at")) {
            auto seq = cam("look_at");
            look_at = vector(
                seq[0u].val().to_double(),
                seq[1u].val().to_double(),
                seq[2u].val().to_double());
        }
        if (cam.has_child("up")) {
            auto seq = cam("up");
            up = vector(
                seq[0u].val().to_double(),
                seq[1u].val().to_double(),
                seq[2u].val().to_double());
        }
        if (cam.has_child("fov")) {
            auto seq = cam("fov");
            fov_h = seq[0u].val().to_double();
            fov_v = seq[1u].val().to_double();
        }
        // build camera
        CLine ray_dir(look_at - position, position);
        COptic optic(ray_dir, up, fov_h, fov_v);
        cameras.clear();
        cameras.emplace_back(new CCamera(optic, &studio, out_fname, imgType, width, height));
    }

    return true;
}