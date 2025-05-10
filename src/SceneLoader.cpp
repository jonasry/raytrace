// SceneLoader.cpp
// Implements scene loading from YAML using RapidYAML

#include "SceneLoader.hpp"
#include "testapp.h"      // for SetupStudio
#include "storage.h"      // for CStorage, ImgClass
// STL for string conversions
#include <string>
#include "camera.h"
#include "line.h"
#include "optic.h"
#include "vector.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define RYML_SINGLE_HDR_DEFINE_NOW
#include "ryml_all.hpp"

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
    // parse with filename and content
    // parse with filename (csubstr) and source (substr)
    ryml::Tree tree = ryml::parse_in_place(
        c4::to_csubstr(filename),
        c4::to_substr(data)
    );
    auto root = tree.rootref();

    // 1. Globals
    if (root.has_child("globals")) {
        auto gl = root["globals"];
        if (gl.has_child("recursion_depth")) {
            auto cs = gl["recursion_depth"].val();
            studio.RecurseDepth = std::stoi(std::string(cs.str, cs.len));
        }
    }

    // 2. Setup default scene (textures, lights, objects)
    SetupStudio(studio);

    // 3. Output parameters
    std::string out_fname = "output.png";
    int width = 200, height = 160;
    CStorage::ImgClass imgType = CStorage::PNG;
    if (root.has_child("output")) {
        auto out = root["output"];
        if (out.has_child("filename")) {
            auto cs = out["filename"].val();
            out_fname = std::string(cs.str, cs.len);
        }
        if (out.has_child("format")) {
            auto cs = out["format"].val();
            auto fmt = std::string(cs.str, cs.len);
            if (fmt == "PNG") imgType = CStorage::PNG;
            else if (fmt == "JPG" || fmt == "JPEG") imgType = CStorage::JPG;
            else if (fmt == "TGA") imgType = CStorage::TGA;
            else if (fmt == "HDR") imgType = CStorage::HDR;
        }
        if (out.has_child("resolution")) {
            auto res = out["resolution"];
            auto cs0 = res[0].val();
            auto cs1 = res[1].val();
            width  = std::stoi(std::string(cs0.str, cs0.len));
            height = std::stoi(std::string(cs1.str, cs1.len));
        }
    }

    // 4. Camera
    if (root.has_child("camera")) {
        auto cam = root["camera"];
        // only perspective supported
        vector position(0,0,0), look_at(0,0,1), up(0,1,0);
        double fov_h = 40.0, fov_v = 40.0;
        if (cam.has_child("position")) {
            auto seq = cam["position"];
            auto c0 = seq[0].val();
            auto c1 = seq[1].val();
            auto c2 = seq[2].val();
            position = vector(
                std::stod(std::string(c0.str, c0.len)),
                std::stod(std::string(c1.str, c1.len)),
                std::stod(std::string(c2.str, c2.len)));
        }
        if (cam.has_child("look_at")) {
            auto seq = cam["look_at"];
            auto c0 = seq[0].val();
            auto c1 = seq[1].val();
            auto c2 = seq[2].val();
            look_at = vector(
                std::stod(std::string(c0.str, c0.len)),
                std::stod(std::string(c1.str, c1.len)),
                std::stod(std::string(c2.str, c2.len)));
        }
        if (cam.has_child("up")) {
            auto seq = cam["up"];
            auto c0 = seq[0].val();
            auto c1 = seq[1].val();
            auto c2 = seq[2].val();
            up = vector(
                std::stod(std::string(c0.str, c0.len)),
                std::stod(std::string(c1.str, c1.len)),
                std::stod(std::string(c2.str, c2.len)));
        }
        if (cam.has_child("fov")) {
            auto seq = cam["fov"];
            auto c0 = seq[0].val();
            auto c1 = seq[1].val();
            fov_h = std::stod(std::string(c0.str, c0.len));
            fov_v = std::stod(std::string(c1.str, c1.len));
        }
        // build camera
        CLine ray_dir(look_at - position, position);
        COptic optic(ray_dir, up, fov_h, fov_v);
        cameras.clear();
        cameras.emplace_back(new CCamera(optic, &studio, out_fname, imgType, width, height));
    }

    return true;
}