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
#include <limits>
#include <unordered_set>
#include <cctype>

#define RYML_SINGLE_HDR_DEFINE_NOW
#include "ryml_all.hpp" // This should include necessary rapidyaml headers for operator>>

namespace {
std::string keyName(const c4::yml::ConstNodeRef& node, const char* fallback = "unknown") {
    if (!node.has_key()) {
        return fallback;
    }
    return std::string(node.key().str, node.key().len);
}

bool requireMapNode(const c4::yml::ConstNodeRef& node, const char* context) {
    if (!node.is_map()) {
        std::cerr << "Error: " << context << " must be a YAML map.\n";
        return false;
    }
    return true;
}

bool requireSeqNode(const c4::yml::ConstNodeRef& node, const char* context) {
    if (!node.is_seq()) {
        std::cerr << "Error: " << context << " must be a YAML sequence.\n";
        return false;
    }
    return true;
}

bool validateKnownKeys(const c4::yml::ConstNodeRef& node,
                       std::initializer_list<const char*> allowedKeys,
                       const char* context) {
    if (!requireMapNode(node, context)) {
        return false;
    }

    std::unordered_set<std::string> allowed;
    for (const char* key : allowedKeys) {
        allowed.emplace(key);
    }

    for (auto child : node.children()) {
        if (!child.has_key()) {
            std::cerr << "Error: " << context << " contains a keyless entry.\n";
            return false;
        }
        std::string key(child.key().str, child.key().len);
        if (allowed.find(key) == allowed.end()) {
            std::cerr << "Error: unknown field '" << key << "' in " << context << ".\n";
            return false;
        }
    }
    return true;
}

template <typename T>
bool readRequiredScalar(const c4::yml::ConstNodeRef& parent, const char* key, T& out, const char* context) {
    if (!parent.has_child(key)) {
        std::cerr << "Error: missing required field '" << key << "' in " << context << ".\n";
        return false;
    }
    auto node = parent[key];
    if (!node.readable()) {
        std::cerr << "Error: field '" << key << "' in " << context << " is not readable.\n";
        return false;
    }
    node >> out;
    return true;
}

template <typename T>
bool readOptionalScalar(const c4::yml::ConstNodeRef& parent, const char* key, T& out, const char* context) {
    if (!parent.has_child(key)) {
        return true;
    }
    auto node = parent[key];
    if (!node.readable()) {
        std::cerr << "Error: field '" << key << "' in " << context << " is not readable.\n";
        return false;
    }
    node >> out;
    return true;
}

bool readVectorNode(const c4::yml::ConstNodeRef& node, CVector& out) {
    if (!node.is_seq() || node.num_children() != 3) {
        std::cerr << "Error: YAML node '" << keyName(node) << "' is not a valid 3-element sequence for a vector.\n";
        return false;
    }
    if (!node[0].readable() || !node[1].readable() || !node[2].readable()) {
        std::cerr << "Error: YAML vector node '" << keyName(node) << "' contains unreadable values.\n";
        return false;
    }
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    node[0] >> x;
    node[1] >> y;
    node[2] >> z;
    out = CVector(x, y, z);
    return true;
}

bool readColorNode(const c4::yml::ConstNodeRef& node, CColor& out) {
    if (!node.is_seq() || node.num_children() != 3) {
        std::cerr << "Error: YAML node '" << keyName(node) << "' is not a valid 3-element sequence for a color.\n";
        return false;
    }
    if (!node[0].readable() || !node[1].readable() || !node[2].readable()) {
        std::cerr << "Error: YAML color node '" << keyName(node) << "' contains unreadable values.\n";
        return false;
    }
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    node[0] >> r;
    node[1] >> g;
    node[2] >> b;
    out = CColor(r, g, b);
    return true;
}

bool readRequiredVector(const c4::yml::ConstNodeRef& parent, const char* key, CVector& out, const char* context) {
    if (!parent.has_child(key)) {
        std::cerr << "Error: missing required field '" << key << "' in " << context << ".\n";
        return false;
    }
    return readVectorNode(parent[key], out);
}

bool readRequiredColor(const c4::yml::ConstNodeRef& parent, const char* key, CColor& out, const char* context) {
    if (!parent.has_child(key)) {
        std::cerr << "Error: missing required field '" << key << "' in " << context << ".\n";
        return false;
    }
    return readColorNode(parent[key], out);
}

bool validateTransforms(const c4::yml::ConstNodeRef& objectNode, const char* context) {
    if (!objectNode.has_child("transforms")) {
        return true;
    }

    auto transforms = objectNode["transforms"];
    if (!requireSeqNode(transforms, "object.transforms")) {
        return false;
    }

    for (auto entry : transforms) {
        if (!requireMapNode(entry, "object transform entry")) {
            return false;
        }
        if (entry.num_children() != 1) {
            std::cerr << "Error: each transform entry must contain exactly one operation in " << context << ".\n";
            return false;
        }

        auto op = entry[0];
        if (!op.has_key()) {
            std::cerr << "Error: transform entry is missing operation name in " << context << ".\n";
            return false;
        }
        std::string opName(op.key().str, op.key().len);
        if (opName != "translate" && opName != "rotate" && opName != "scale") {
            std::cerr << "Error: unsupported transform '" << opName << "' in " << context << ".\n";
            return false;
        }

        CVector v(0, 0, 0);
        if (!readVectorNode(op, v)) {
            return false;
        }
    }

    return true;
}
} // namespace

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
    if (!validateKnownKeys(root, {"globals", "textures", "objects", "output", "camera"}, "scene root")) {
        return false;
    }

    // Build into a temporary studio so parse failures do not mutate caller state.
    CStudio stagedStudio(studio.RecurseDepth);
    stagedStudio.RecurseDepth = studio.RecurseDepth;
    stagedStudio.Oversampling = studio.Oversampling;
    stagedStudio.BackgroundColor = studio.BackgroundColor;

    // 1. Globals
    if (root.has_child("globals")) {
        auto gl = root["globals"];
        if (!validateKnownKeys(gl, {"recursion_depth", "oversampling", "background_color"}, "globals")) {
            return false;
        }

        int recursionDepth = stagedStudio.RecurseDepth;
        if (!readOptionalScalar(gl, "recursion_depth", recursionDepth, "globals")) {
            return false;
        }
        if (recursionDepth < 0) {
            std::cerr << "Error: recursion_depth must be >= 0. Got " << recursionDepth << ".\n";
            return false;
        }
        stagedStudio.RecurseDepth = recursionDepth;

        int oversampling = stagedStudio.Oversampling;
        if (!readOptionalScalar(gl, "oversampling", oversampling, "globals")) {
            return false;
        }
        if (oversampling < 1) {
            std::cerr << "Error: oversampling must be >= 1. Got " << oversampling << ".\n";
            return false;
        }
        stagedStudio.Oversampling = oversampling;

        if (gl.has_child("background_color")) {
            CColor color(0, 0, 0);
            if (!readColorNode(gl["background_color"], color)) {
                return false;
            }
            stagedStudio.BackgroundColor = color;
        }
    }

    // 2. Textures and objects
    // Map texture names to raw pointers (stagedStudio owns the textures)
    std::unordered_map<std::string, CTexture*> textureMap;
    if (root.has_child("textures")) {
        auto textures = root["textures"];
        if (!requireSeqNode(textures, "textures")) {
            return false;
        }
        for (auto tex : textures) {
            if (!validateKnownKeys(tex, {"type", "name", "color"}, "texture")) {
                return false;
            }
            std::string type;
            if (!readRequiredScalar(tex, "type", type, "texture")) {
                return false;
            }
            if (type != "solid") {
                std::cerr << "Error: unsupported texture type '" << type << "'.\n";
                return false;
            }

            std::string name;
            CColor color(0, 0, 0);
            if (!readRequiredScalar(tex, "name", name, "texture")) {
                return false;
            }
            if (!readRequiredColor(tex, "color", color, "texture")) {
                return false;
            }
            if (textureMap.find(name) != textureMap.end()) {
                std::cerr << "Error: duplicate texture name '" << name << "'.\n";
                return false;
            }

            stagedStudio.Textures.emplace_back(new CTexture(color));
            textureMap[name] = stagedStudio.Textures.back().get();
        }
    }

    if (root.has_child("objects")) {
        auto objects = root["objects"];
        if (!requireSeqNode(objects, "objects")) {
            return false;
        }
        for (auto obj : objects) {
            if (!requireMapNode(obj, "object")) {
                return false;
            }
            std::string type;
            if (!readRequiredScalar(obj, "type", type, "object")) {
                return false;
            }
            if (type == "plane") {
                if (!validateKnownKeys(obj, {"id", "type", "point", "normal", "texture", "transforms"}, "plane object")) {
                    return false;
                }
                CVector point(0, 0, 0);
                CVector normal(0, 0, 1);
                std::string texture_name;
                if (!readRequiredVector(obj, "point", point, "plane object")) {
                    return false;
                }
                if (!readRequiredVector(obj, "normal", normal, "plane object")) {
                    return false;
                }
                if (!readRequiredScalar(obj, "texture", texture_name, "plane object")) {
                    return false;
                }
                auto it = textureMap.find(texture_name);
                if (it == textureMap.end()) {
                    std::cerr << "Error: texture '" << texture_name << "' not found for plane object.\n";
                    return false;
                }
                if (!validateTransforms(obj, "plane object")) {
                    return false;
                }
                stagedStudio.Objects.Objects.push_back(new CPlane(point, normal, it->second, nullptr, false));
            } else if (type == "sphere") {
                if (!validateKnownKeys(obj, {"id", "type", "center", "radius", "texture", "transforms"}, "sphere object")) {
                    return false;
                }
                CVector center(0, 0, 0);
                double radius = 0.0;
                std::string texture_name;
                if (!readRequiredVector(obj, "center", center, "sphere object")) {
                    return false;
                }
                if (!readRequiredScalar(obj, "radius", radius, "sphere object")) {
                    return false;
                }
                if (radius <= 0.0) {
                    std::cerr << "Error: sphere radius must be > 0. Got " << radius << ".\n";
                    return false;
                }
                if (!readRequiredScalar(obj, "texture", texture_name, "sphere object")) {
                    return false;
                }
                auto it = textureMap.find(texture_name);
                if (it == textureMap.end()) {
                    std::cerr << "Error: texture '" << texture_name << "' not found for sphere object.\n";
                    return false;
                }
                if (!validateTransforms(obj, "sphere object")) {
                    return false;
                }
                stagedStudio.Objects.Objects.push_back(new CSphere(radius, center, it->second, nullptr, false));
            } else {
                std::cerr << "Error: unsupported object type '" << type << "'.\n";
                return false;
            }
        }
        SetupLights(stagedStudio);
    } else {
        SetupStudio(stagedStudio);
    }

    // 3. Output parameters
    std::string out_fname = "output.png";
    int width = 200, height = 160;
    CStorage::ImgClass imgType = CStorage::PNG;
    if (root.has_child("output")) {
        auto out = root["output"];
        if (!validateKnownKeys(out, {"filename", "format", "resolution"}, "output")) {
            return false;
        }
        if (!readOptionalScalar(out, "filename", out_fname, "output")) {
            return false;
        }
        if (out_fname.empty()) {
            std::cerr << "Error: output filename must not be empty.\n";
            return false;
        }
        std::string fmt;
        if (!readOptionalScalar(out, "format", fmt, "output")) {
            return false;
        }
        if (!fmt.empty()) {
            for (char& c : fmt) {
                c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            }
            if (fmt == "PNG") imgType = CStorage::PNG;
            else if (fmt == "JPG" || fmt == "JPEG") imgType = CStorage::JPG;
            else if (fmt == "TGA") imgType = CStorage::TGA;
            else if (fmt == "HDR") imgType = CStorage::HDR;
            else {
                std::cerr << "Error: unsupported output format '" << fmt << "'.\n";
                return false;
            }
        }
        if (out.has_child("resolution")) {
            auto res = out["resolution"];
            if (res.is_seq() && res.num_children() == 2) {
                if (!res[0].readable() || !res[1].readable()) {
                    std::cerr << "Error: Output resolution values are not readable.\n";
                    return false;
                }

                int requestedWidth = 0;
                int requestedHeight = 0;
                res[0] >> requestedWidth;
                res[1] >> requestedHeight;

                if (requestedWidth <= 0 || requestedHeight <= 0) {
                    std::cerr << "Error: Output resolution must be positive. Got ["
                              << requestedWidth << ", " << requestedHeight << "].\n";
                    return false;
                }

                constexpr int kMaxScoord = std::numeric_limits<scoord>::max();
                if (requestedWidth > kMaxScoord || requestedHeight > kMaxScoord) {
                    std::cerr << "Error: Output resolution exceeds max supported size (" << kMaxScoord
                              << "). Got [" << requestedWidth << ", " << requestedHeight << "].\n";
                    return false;
                }

                width = requestedWidth;
                height = requestedHeight;
            } else {
                std::cerr << "Error: Output resolution is not a valid 2-element sequence.\n";
                return false;
            }
        }
    }

    // 4. Camera
    bool hasCamera = false;
    vector position(0,0,0), look_at(0,0,1), up(0,1,0);
    double fov_h = 40.0, fov_v = 40.0;
    if (root.has_child("camera")) {
        hasCamera = true;
        auto cam = root["camera"];
        if (!validateKnownKeys(cam, {"type", "position", "look_at", "up", "fov"}, "camera")) {
            return false;
        }

        std::string camType;
        if (!readOptionalScalar(cam, "type", camType, "camera")) {
            return false;
        }
        if (!camType.empty()) {
            for (char& c : camType) {
                c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            }
            if (camType != "perspective") {
                std::cerr << "Error: unsupported camera type '" << camType << "'.\n";
                return false;
            }
        }

        if (cam.has_child("position")) {
            if (!readVectorNode(cam["position"], position)) {
                return false;
            }
        }
        if (cam.has_child("look_at")) {
            if (!readVectorNode(cam["look_at"], look_at)) {
                return false;
            }
        }
        if (cam.has_child("up")) {
            if (!readVectorNode(cam["up"], up)) {
                return false;
            }
        }
        if (cam.has_child("fov")) {
            auto fov_node = cam["fov"];
            if (fov_node.is_seq() && fov_node.num_children() == 2) {
                if (!fov_node[0].readable() || !fov_node[1].readable()) {
                    std::cerr << "Error: Camera FOV values are not readable.\n";
                    return false;
                }
                fov_node[0] >> fov_h;
                fov_node[1] >> fov_v;
                if (fov_h <= 0.0 || fov_h >= 180.0 || fov_v <= 0.0 || fov_v >= 180.0) {
                    std::cerr << "Error: Camera FOV values must be in (0, 180). Got ["
                              << fov_h << ", " << fov_v << "].\n";
                    return false;
                }
            } else {
                std::cerr << "Error: Camera FOV is not a valid 2-element sequence.\n";
                return false;
            }
        }

        if ((look_at - position).NormOf() <= 0.0) {
            std::cerr << "Error: camera look_at must differ from position.\n";
            return false;
        }
        if (up.NormOf() <= 0.0) {
            std::cerr << "Error: camera up vector must be non-zero.\n";
            return false;
        }
    }

    // Commit scene only after full parse/validation succeeds.
    studio.Lights.swap(stagedStudio.Lights);
    studio.Textures.swap(stagedStudio.Textures);
    studio.Objects.Objects.swap(stagedStudio.Objects.Objects);
    studio.BackgroundColor = stagedStudio.BackgroundColor;
    studio.RecurseDepth = stagedStudio.RecurseDepth;
    studio.Oversampling = stagedStudio.Oversampling;

    if (hasCamera) {
        CLine ray_dir(look_at - position, position);
        COptic optic(ray_dir, up, fov_h, fov_v);
        std::vector<std::unique_ptr<CCamera>> stagedCameras;
        stagedCameras.emplace_back(new CCamera(optic, &studio, out_fname, imgType, width, height));
        cameras.swap(stagedCameras);
    }

    return true;
}
