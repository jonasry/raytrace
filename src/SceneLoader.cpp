// SceneLoader.cpp
// Implements scene loading from YAML using RapidYAML

#include "SceneLoader.hpp"
#include "testapp.h"      // for SetupStudio
#include "storage.h"      // for CStorage, ImgClass
#include <string>
#include <unordered_map>
// STL for string conversions
#include "camera.h"
#include "line.h"
#include "optic.h"
#include "vector.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define RYML_SINGLE_HDR_DEFINE_NOW
#include "ryml_all.hpp" // This should include necessary rapidyaml headers for operator>>

// getString function is removed

CVector getVector(const c4::yml::NodeRef& nodeRef) {
    float x, y, z;
    if (nodeRef.is_seq() && nodeRef.num_children() == 3) {
        nodeRef[0] >> x;
        nodeRef[1] >> y;
        nodeRef[2] >> z;
        return CVector(x, y, z);
    }
    // It's good practice to output the key of the problematic node if available
    // Convert key to std::string for printing, if it exists
    std::string key_name = "unknown_key";
    if (nodeRef.has_key()) {
        key_name = std::string(nodeRef.key().str, nodeRef.key().len);
    }
    std::cerr << "Error: YAML node '" << key_name << "' is not a valid 3-element sequence for a vector." << std::endl;
    return CVector(); // Default CVector
}

CColor getColor(const c4::yml::NodeRef& nodeRef) {
    float r, g, b;
    if (nodeRef.is_seq() && nodeRef.num_children() == 3) {
        nodeRef[0] >> r;
        nodeRef[1] >> g;
        nodeRef[2] >> b;
        return CColor(r, g, b);
    }
    std::string key_name = "unknown_key";
    if (nodeRef.has_key()) {
        key_name = std::string(nodeRef.key().str, nodeRef.key().len);
    }
    std::cerr << "Error: YAML node '" << key_name << "' is not a valid 3-element sequence for a color." << std::endl;
    return CColor(); // Default CColor
}

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
            gl["recursion_depth"] >> studio.RecurseDepth;
        }
    }

    // 2. Setup default scene (textures, lights, objects)
    // Map texture names to raw pointers (studio owns the textures)
    std::unordered_map<std::string, CTexture*> textureMap;
    if (root.has_child("textures")) {
        auto textures = root["textures"];
        for (auto tex : textures) {
            std::string type;
            tex["type"] >> type;
            if (type == "solid") {
                std::string name;
                tex["name"] >> name;
                auto color = getColor(tex["color"]);
                auto texture = new CTexture(color);
                studio.Textures.emplace_back(texture);
                textureMap[name] = texture;
            }
        }
    }
    
    if (root.has_child("objects")) {
        auto objects = root["objects"];
        for (auto obj : objects) {
            auto tval = obj["type"];
            std::string type;
            tval >> type;
            if (type == "plane") {
                auto point = getVector(obj["point"]);
                auto normal = getVector(obj["normal"]);
                std::string texture_name;
                obj["texture"] >> texture_name;
                auto it = textureMap.find(texture_name);
                if (it == textureMap.end()) {
                    std::cerr << "Texture '" << texture_name << "' not found\n";
                    continue;
                }
                CPlane* P = new CPlane(point, normal, it->second, nullptr, false);
                studio.Objects.Objects.push_back(P);

            } else if (type == "sphere") {
                auto center = getVector(obj["center"]);
                double radius;
                obj["radius"] >> radius;
                std::string texture_name;
                obj["texture"] >> texture_name;
                auto it = textureMap.find(texture_name);
                if (it == textureMap.end()) {
                    std::cerr << "Texture '" << texture_name << "' not found\n";
                    continue;
                }
                CSphere* sph = new CSphere(radius, center, it->second, nullptr, false);
                studio.Objects.Objects.push_back(sph);
            }
        }
    	SetupLights(studio);

    } else {
        SetupStudio(studio);
    }

    // 3. Output parameters
    std::string out_fname = "output.png";
    int width = 200, height = 160;
    CStorage::ImgClass imgType = CStorage::PNG;
    if (root.has_child("output")) {
        auto out = root["output"];
        if (out.has_child("filename")) {
            out["filename"] >> out_fname;
        }
        if (out.has_child("format")) {
            std::string fmt;
            out["format"] >> fmt;
            if (fmt == "PNG") imgType = CStorage::PNG;
            else if (fmt == "JPG" || fmt == "JPEG") imgType = CStorage::JPG;
            else if (fmt == "TGA") imgType = CStorage::TGA;
            else if (fmt == "HDR") imgType = CStorage::HDR;
        }
        if (out.has_child("resolution")) {
            auto res = out["resolution"];
            // Ensure resolution is a sequence with 2 elements before deserializing
            if (res.is_seq() && res.num_children() == 2) {
                res[0] >> width;
                res[1] >> height;
            } else {
                 std::cerr << "Error: Output resolution is not a valid 2-element sequence." << std::endl;
                 // Keep default width/height or handle error as appropriate
            }
        }
    }

    // 4. Camera
    if (root.has_child("camera")) {
        auto cam = root["camera"];
        // only perspective supported
        vector position(0,0,0), look_at(0,0,1), up(0,1,0);
        double fov_h = 40.0, fov_v = 40.0;
        if (cam.has_child("position")) {
            // getVector will handle parsing and errors for position
            position = getVector(cam["position"]);
        }
        if (cam.has_child("look_at")) {
            // getVector will handle parsing and errors for look_at
            look_at = getVector(cam["look_at"]);
        }
        if (cam.has_child("up")) {
            // getVector will handle parsing and errors for up
            up = getVector(cam["up"]);
        }
        if (cam.has_child("fov")) {
            auto fov_node = cam["fov"];
            if (fov_node.is_seq() && fov_node.num_children() == 2) {
                fov_node[0] >> fov_h;
                fov_node[1] >> fov_v;
            } else {
                std::cerr << "Error: Camera FOV is not a valid 2-element sequence." << std::endl;
                // Keep default fov_h, fov_v or handle error as appropriate
            }
        }
        // build camera
        CLine ray_dir(look_at - position, position);
        COptic optic(ray_dir, up, fov_h, fov_v);
        cameras.clear();
        cameras.emplace_back(new CCamera(optic, &studio, out_fname, imgType, width, height));
    }

    return true;
}