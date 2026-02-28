// SceneLoader.cpp
// Implements scene loading from YAML using RapidYAML

#include "SceneLoader.hpp"

#include "camera.h"
#include "checker.h"
#include "lights.h"
#include "line.h"
#include "marble.h"
#include "noise.h"
#include "objects.h"
#include "optic.h"
#include "storage.h"
#include "testapp.h"
#include "texture.h"
#include "turb.h"
#include "vector.h"

#include <array>
#include <cmath>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define RYML_SINGLE_HDR_DEFINE_NOW
#include "ryml_all.hpp"

namespace {

enum class TransformType {
    Translate,
    Rotate,
    Scale,
};

struct TransformSpec {
    TransformType type;
    CVector value;
};

struct TextureSpec {
    std::string name;
    std::string type;
    CColor color = CColor(0, 0, 0);
    CColor diffuse = CColor(0, 0, 0);
    CColor specular = CColor(0, 0, 0);
    double shinePow = 0.0;
    double ks = 0.0;
    double ior = 1.0;
    double transparency = 0.0;
    double scale = 1.0;
    CVector cells = CVector(1, 1, 1);
    std::vector<std::string> baseTextures;
};

struct ObjectSpec {
    std::string id;
    std::string type;
    std::string textureName;
    bool flipInside = false;
    CVector point = CVector(0, 0, 0);
    CVector normal = CVector(0, 0, 1);
    CVector center = CVector(0, 0, 0);
    CVector min = CVector(0, 0, 0);
    CVector max = CVector(0, 0, 0);
    CVector base = CVector(0, 0, 0);
    CVector apex = CVector(0, 0, 0);
    double radius = 0.0;
    std::vector<std::string> children;
    std::vector<TransformSpec> transforms;
};

enum class BuildState {
    Unvisited,
    Visiting,
    Done,
};

std::string keyName(const c4::yml::ConstNodeRef& node, const char* fallback = "unknown") {
    if (!node.has_key()) {
        return fallback;
    }
    return std::string(node.key().str, node.key().len);
}

std::string toLowerAscii(std::string value) {
    for (char& c : value) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return value;
}

std::string trimAscii(std::string value) {
    std::size_t begin = 0;
    while (begin < value.size() &&
           std::isspace(static_cast<unsigned char>(value[begin])) != 0) {
        ++begin;
    }

    std::size_t end = value.size();
    while (end > begin &&
           std::isspace(static_cast<unsigned char>(value[end - 1])) != 0) {
        --end;
    }

    return value.substr(begin, end - begin);
}

bool parseToSpecExpression(const std::string& expression, CColor& out) {
    const std::string trimmed = trimAscii(expression);
    const std::string prefix = "to_spec(";
    if (trimmed.size() <= prefix.size() + 1 ||
        trimmed.compare(0, prefix.size(), prefix) != 0 ||
        trimmed.back() != ')') {
        return false;
    }

    std::string body = trimmed.substr(prefix.size(), trimmed.size() - prefix.size() - 1);
    std::array<double, 3> values = {0.0, 0.0, 0.0};
    std::size_t cursor = 0;

    auto skipSpaces = [&](void) {
        while (cursor < body.size() &&
               std::isspace(static_cast<unsigned char>(body[cursor])) != 0) {
            ++cursor;
        }
    };

    for (std::size_t index = 0; index < values.size(); ++index) {
        skipSpaces();
        const char* start = body.c_str() + cursor;
        char* end = nullptr;
        values[index] = std::strtod(start, &end);
        if (end == start) {
            return false;
        }
        cursor += static_cast<std::size_t>(end - start);
        skipSpaces();

        if (index + 1 < values.size()) {
            if (cursor >= body.size() || body[cursor] != ',') {
                return false;
            }
            ++cursor;
        }
    }

    skipSpaces();
    if (cursor != body.size()) {
        return false;
    }

    for (double& value : values) {
        if (value < 0.0 || value >= 1.0) {
            std::cerr << "Error: to_spec() arguments must be in [0, 1). Got "
                      << value << ".\n";
            return false;
        }
        const double root = std::sqrt(value);
        value = (1.0 + root) / (1.0 - root);
    }

    out = CColor(values[0], values[1], values[2]);
    return true;
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
bool readRequiredScalar(const c4::yml::ConstNodeRef& parent,
                        const char* key,
                        T& out,
                        const char* context) {
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
bool readOptionalScalar(const c4::yml::ConstNodeRef& parent,
                        const char* key,
                        T& out,
                        const char* context) {
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
        std::cerr << "Error: YAML node '" << keyName(node)
                  << "' is not a valid 3-element sequence for a vector.\n";
        return false;
    }
    if (!node[0].readable() || !node[1].readable() || !node[2].readable()) {
        std::cerr << "Error: YAML vector node '" << keyName(node)
                  << "' contains unreadable values.\n";
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
    if (!node.is_seq()) {
        if (!node.readable()) {
            std::cerr << "Error: YAML color node '" << keyName(node)
                      << "' is not readable.\n";
            return false;
        }

        std::string expression(node.val().str, node.val().len);
        if (parseToSpecExpression(expression, out)) {
            return true;
        }

        std::cerr << "Error: YAML node '" << keyName(node)
                  << "' is not a valid 3-element color or to_spec(...).\n";
        return false;
    }

    if (!node.is_seq() || node.num_children() != 3) {
        std::cerr << "Error: YAML node '" << keyName(node)
                  << "' is not a valid 3-element sequence for a color.\n";
        return false;
    }
    if (!node[0].readable() || !node[1].readable() || !node[2].readable()) {
        std::cerr << "Error: YAML color node '" << keyName(node)
                  << "' contains unreadable values.\n";
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

bool readRequiredVector(const c4::yml::ConstNodeRef& parent,
                        const char* key,
                        CVector& out,
                        const char* context) {
    if (!parent.has_child(key)) {
        std::cerr << "Error: missing required field '" << key << "' in " << context << ".\n";
        return false;
    }
    return readVectorNode(parent[key], out);
}

bool readRequiredColor(const c4::yml::ConstNodeRef& parent,
                       const char* key,
                       CColor& out,
                       const char* context) {
    if (!parent.has_child(key)) {
        std::cerr << "Error: missing required field '" << key << "' in " << context << ".\n";
        return false;
    }
    return readColorNode(parent[key], out);
}

bool readOptionalVector(const c4::yml::ConstNodeRef& parent, const char* key, CVector& out) {
    if (!parent.has_child(key)) {
        return true;
    }
    return readVectorNode(parent[key], out);
}

bool readOptionalColor(const c4::yml::ConstNodeRef& parent, const char* key, CColor& out) {
    if (!parent.has_child(key)) {
        return true;
    }
    return readColorNode(parent[key], out);
}

bool readStringSequence(const c4::yml::ConstNodeRef& parent,
                        const char* key,
                        std::vector<std::string>& out,
                        const char* context) {
    if (!parent.has_child(key)) {
        std::cerr << "Error: missing required field '" << key << "' in " << context << ".\n";
        return false;
    }

    auto seq = parent[key];
    if (!requireSeqNode(seq, key)) {
        return false;
    }

    out.clear();
    for (auto item : seq) {
        if (!item.readable()) {
            std::cerr << "Error: sequence '" << key << "' in " << context
                      << " contains an unreadable value.\n";
            return false;
        }
        std::string value;
        item >> value;
        out.push_back(value);
    }
    return true;
}

bool readTransforms(const c4::yml::ConstNodeRef& objectNode,
                    std::vector<TransformSpec>& out,
                    const char* context) {
    out.clear();
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
            std::cerr << "Error: each transform entry must contain exactly one operation in "
                      << context << ".\n";
            return false;
        }

        auto op = entry[0];
        if (!op.has_key()) {
            std::cerr << "Error: transform entry is missing operation name in "
                      << context << ".\n";
            return false;
        }

        TransformSpec spec{};
        const std::string opName = std::string(op.key().str, op.key().len);
        if (opName == "translate") {
            spec.type = TransformType::Translate;
        } else if (opName == "rotate") {
            spec.type = TransformType::Rotate;
        } else if (opName == "scale") {
            spec.type = TransformType::Scale;
        } else {
            std::cerr << "Error: unsupported transform '" << opName << "' in "
                      << context << ".\n";
            return false;
        }

        if (!readVectorNode(op, spec.value)) {
            return false;
        }
        out.push_back(spec);
    }

    return true;
}

void applyTransforms(CPrimitive& object, const std::vector<TransformSpec>& transforms) {
    for (const TransformSpec& transform : transforms) {
        switch (transform.type) {
            case TransformType::Translate:
                object.Translate(transform.value);
                break;
            case TransformType::Rotate:
                object.Rotate(transform.value);
                break;
            case TransformType::Scale:
                object.Scale(transform.value);
                break;
        }
    }
}

bool parseTextureSpecs(const c4::yml::ConstNodeRef& texturesNode,
                       std::vector<TextureSpec>& orderedSpecs,
                       std::unordered_map<std::string, std::size_t>& indexByName) {
    if (!requireSeqNode(texturesNode, "textures")) {
        return false;
    }

    orderedSpecs.clear();
    indexByName.clear();

    for (auto tex : texturesNode) {
        if (!requireMapNode(tex, "texture")) {
            return false;
        }

        TextureSpec spec;
        if (!readRequiredScalar(tex, "name", spec.name, "texture")) {
            return false;
        }
        if (indexByName.find(spec.name) != indexByName.end()) {
            std::cerr << "Error: duplicate texture name '" << spec.name << "'.\n";
            return false;
        }
        if (!readRequiredScalar(tex, "type", spec.type, "texture")) {
            return false;
        }
        spec.type = toLowerAscii(spec.type);

        if (spec.type == "solid") {
            if (!validateKnownKeys(tex, {"name", "type", "color"}, "solid texture")) {
                return false;
            }
            if (!readRequiredColor(tex, "color", spec.color, "solid texture")) {
                return false;
            }
        } else if (spec.type == "material") {
            if (!validateKnownKeys(tex, {"name", "type", "diffuse", "specular",
                                         "shine_pow", "ks", "ior", "transparency"},
                                   "material texture")) {
                return false;
            }
            if (!readRequiredColor(tex, "diffuse", spec.diffuse, "material texture")) {
                return false;
            }
            if (!readOptionalColor(tex, "specular", spec.specular)) {
                return false;
            }
            if (!readOptionalScalar(tex, "shine_pow", spec.shinePow, "material texture")) {
                return false;
            }
            if (!readOptionalScalar(tex, "ks", spec.ks, "material texture")) {
                return false;
            }
            if (!readOptionalScalar(tex, "ior", spec.ior, "material texture")) {
                return false;
            }
            if (!readOptionalScalar(tex, "transparency", spec.transparency,
                                    "material texture")) {
                return false;
            }
            if (spec.ior <= 0.0) {
                std::cerr << "Error: material texture '" << spec.name
                          << "' must have ior > 0.\n";
                return false;
            }
            if (spec.transparency < 0.0 || spec.transparency > 1.0) {
                std::cerr << "Error: material texture '" << spec.name
                          << "' must have transparency in [0, 1].\n";
                return false;
            }
        } else if (spec.type == "checker") {
            if (!validateKnownKeys(tex, {"name", "type", "base_textures", "cells"},
                                   "checker texture")) {
                return false;
            }
            if (!readStringSequence(tex, "base_textures", spec.baseTextures, "checker texture")) {
                return false;
            }
            if (spec.baseTextures.size() != 2) {
                std::cerr << "Error: checker texture '" << spec.name
                          << "' requires exactly 2 base_textures.\n";
                return false;
            }
            if (!readOptionalVector(tex, "cells", spec.cells)) {
                return false;
            }
        } else if (spec.type == "noise" || spec.type == "turbulence" || spec.type == "marble") {
            if (!validateKnownKeys(tex, {"name", "type", "base_textures", "scale"},
                                   "procedural texture")) {
                return false;
            }
            if (!readStringSequence(tex, "base_textures", spec.baseTextures,
                                    "procedural texture")) {
                return false;
            }
            if (spec.baseTextures.size() != 2) {
                std::cerr << "Error: texture '" << spec.name
                          << "' requires exactly 2 base_textures.\n";
                return false;
            }
            if (!readOptionalScalar(tex, "scale", spec.scale, "procedural texture")) {
                return false;
            }
            if (spec.scale <= 0.0) {
                std::cerr << "Error: texture '" << spec.name << "' must have scale > 0.\n";
                return false;
            }
        } else {
            std::cerr << "Error: unsupported texture type '" << spec.type << "'.\n";
            return false;
        }

        indexByName.emplace(spec.name, orderedSpecs.size());
        orderedSpecs.push_back(spec);
    }

    return true;
}

bool buildTextures(const std::vector<TextureSpec>& orderedSpecs,
                   const std::unordered_map<std::string, std::size_t>& indexByName,
                   CStudio& stagedStudio,
                   std::unordered_map<std::string, CTexture*>& textureMap) {
    textureMap.clear();
    std::unordered_map<std::string, BuildState> buildState;

    std::function<CTexture*(const std::string&)> buildTexture =
        [&](const std::string& name) -> CTexture* {
            auto built = textureMap.find(name);
            if (built != textureMap.end()) {
                return built->second;
            }

            auto indexIt = indexByName.find(name);
            if (indexIt == indexByName.end()) {
                std::cerr << "Error: texture '" << name << "' was referenced but not defined.\n";
                return nullptr;
            }

            BuildState& state = buildState[name];
            if (state == BuildState::Visiting) {
                std::cerr << "Error: cyclic texture dependency involving '" << name << "'.\n";
                return nullptr;
            }
            if (state == BuildState::Done) {
                auto finished = textureMap.find(name);
                return finished == textureMap.end() ? nullptr : finished->second;
            }

            state = BuildState::Visiting;
            const TextureSpec& spec = orderedSpecs[indexIt->second];
            std::unique_ptr<CTexture> texture;

            if (spec.type == "solid") {
                texture.reset(new CTexture(spec.color));
            } else if (spec.type == "material") {
                texture.reset(new CTexture(spec.diffuse, spec.specular, spec.shinePow,
                                           spec.ks, spec.ior, spec.transparency));
            } else if (spec.type == "checker") {
                CTexture* first = buildTexture(spec.baseTextures[0]);
                CTexture* second = buildTexture(spec.baseTextures[1]);
                if (first == nullptr || second == nullptr) {
                    return nullptr;
                }
                texture.reset(new CChecker(first, second,
                                           spec.cells.GetX(),
                                           spec.cells.GetY(),
                                           spec.cells.GetZ()));
            } else if (spec.type == "noise") {
                CTexture* first = buildTexture(spec.baseTextures[0]);
                CTexture* second = buildTexture(spec.baseTextures[1]);
                if (first == nullptr || second == nullptr) {
                    return nullptr;
                }
                texture.reset(new CNoise(first, second, spec.scale));
            } else if (spec.type == "turbulence") {
                CTexture* first = buildTexture(spec.baseTextures[0]);
                CTexture* second = buildTexture(spec.baseTextures[1]);
                if (first == nullptr || second == nullptr) {
                    return nullptr;
                }
                texture.reset(new CTurbulence(first, second, spec.scale));
            } else if (spec.type == "marble") {
                CTexture* first = buildTexture(spec.baseTextures[0]);
                CTexture* second = buildTexture(spec.baseTextures[1]);
                if (first == nullptr || second == nullptr) {
                    return nullptr;
                }
                texture.reset(new CMarble(first, second, spec.scale));
            } else {
                std::cerr << "Error: unsupported texture type '" << spec.type << "'.\n";
                return nullptr;
            }

            stagedStudio.Textures.emplace_back(std::move(texture));
            textureMap[name] = stagedStudio.Textures.back().get();
            state = BuildState::Done;
            return textureMap[name];
        };

    for (const TextureSpec& spec : orderedSpecs) {
        if (buildTexture(spec.name) == nullptr) {
            return false;
        }
    }

    return true;
}

bool parseObjectSpecs(const c4::yml::ConstNodeRef& objectsNode,
                      std::vector<ObjectSpec>& orderedSpecs,
                      std::unordered_map<std::string, std::size_t>& indexById,
                      std::unordered_map<std::string, std::size_t>& childUseCount) {
    if (!requireSeqNode(objectsNode, "objects")) {
        return false;
    }

    orderedSpecs.clear();
    indexById.clear();
    childUseCount.clear();

    for (auto obj : objectsNode) {
        if (!requireMapNode(obj, "object")) {
            return false;
        }

        ObjectSpec spec;
        if (!readRequiredScalar(obj, "id", spec.id, "object")) {
            return false;
        }
        if (indexById.find(spec.id) != indexById.end()) {
            std::cerr << "Error: duplicate object id '" << spec.id << "'.\n";
            return false;
        }
        if (!readRequiredScalar(obj, "type", spec.type, "object")) {
            return false;
        }
        spec.type = toLowerAscii(spec.type);
        if (!readOptionalScalar(obj, "flip_inside", spec.flipInside, "object")) {
            return false;
        }

        const std::string context = spec.type + " object";
        if (spec.type == "plane") {
            if (!validateKnownKeys(obj, {"id", "type", "point", "normal", "texture",
                                         "transforms", "flip_inside"},
                                   context.c_str())) {
                return false;
            }
            if (!readRequiredVector(obj, "point", spec.point, context.c_str())) {
                return false;
            }
            if (!readRequiredVector(obj, "normal", spec.normal, context.c_str())) {
                return false;
            }
            if (!readRequiredScalar(obj, "texture", spec.textureName, context.c_str())) {
                return false;
            }
        } else if (spec.type == "sphere") {
            if (!validateKnownKeys(obj, {"id", "type", "center", "radius", "texture",
                                         "transforms", "flip_inside"},
                                   context.c_str())) {
                return false;
            }
            if (!readRequiredVector(obj, "center", spec.center, context.c_str())) {
                return false;
            }
            if (!readRequiredScalar(obj, "radius", spec.radius, context.c_str())) {
                return false;
            }
            if (spec.radius <= 0.0) {
                std::cerr << "Error: sphere '" << spec.id << "' must have radius > 0.\n";
                return false;
            }
            if (!readRequiredScalar(obj, "texture", spec.textureName, context.c_str())) {
                return false;
            }
        } else if (spec.type == "box") {
            if (!validateKnownKeys(obj, {"id", "type", "min", "max", "texture",
                                         "transforms", "flip_inside"},
                                   context.c_str())) {
                return false;
            }
            if (!readRequiredVector(obj, "min", spec.min, context.c_str())) {
                return false;
            }
            if (!readRequiredVector(obj, "max", spec.max, context.c_str())) {
                return false;
            }
            if (!readRequiredScalar(obj, "texture", spec.textureName, context.c_str())) {
                return false;
            }
        } else if (spec.type == "cone") {
            if (!validateKnownKeys(obj, {"id", "type", "base", "apex", "radius", "texture",
                                         "transforms", "flip_inside"},
                                   context.c_str())) {
                return false;
            }
            if (!readRequiredVector(obj, "base", spec.base, context.c_str())) {
                return false;
            }
            if (!readRequiredVector(obj, "apex", spec.apex, context.c_str())) {
                return false;
            }
            if (!readRequiredScalar(obj, "radius", spec.radius, context.c_str())) {
                return false;
            }
            if (spec.radius <= 0.0) {
                std::cerr << "Error: cone '" << spec.id << "' must have radius > 0.\n";
                return false;
            }
            if ((spec.apex - spec.base).NormOf() <= 0.0) {
                std::cerr << "Error: cone '" << spec.id << "' apex must differ from base.\n";
                return false;
            }
            if (!readRequiredScalar(obj, "texture", spec.textureName, context.c_str())) {
                return false;
            }
        } else if (spec.type == "group" || spec.type == "intersect" || spec.type == "union") {
            if (!validateKnownKeys(obj, {"id", "type", "children", "transforms", "flip_inside"},
                                   context.c_str())) {
                return false;
            }
            if (!readStringSequence(obj, "children", spec.children, context.c_str())) {
                return false;
            }
            if (spec.type == "group" && spec.flipInside) {
                std::cerr << "Error: group object '" << spec.id
                          << "' does not support flip_inside.\n";
                return false;
            }
        } else {
            std::cerr << "Error: unsupported object type '" << spec.type << "'.\n";
            return false;
        }

        if (!readTransforms(obj, spec.transforms, context.c_str())) {
            return false;
        }

        indexById.emplace(spec.id, orderedSpecs.size());
        orderedSpecs.push_back(spec);
    }

    for (const ObjectSpec& spec : orderedSpecs) {
        for (const std::string& childId : spec.children) {
            const std::size_t count = ++childUseCount[childId];
            if (count > 1) {
                std::cerr << "Error: child object '" << childId
                          << "' is referenced by multiple parents.\n";
                return false;
            }
        }
    }

    for (const auto& entry : childUseCount) {
        if (indexById.find(entry.first) == indexById.end()) {
            std::cerr << "Error: child object '" << entry.first
                      << "' was referenced but not defined.\n";
            return false;
        }
    }

    return true;
}

bool buildObjects(const std::vector<ObjectSpec>& orderedSpecs,
                  const std::unordered_map<std::string, std::size_t>& indexById,
                  const std::unordered_map<std::string, std::size_t>& childUseCount,
                  const std::unordered_map<std::string, CTexture*>& textureMap,
                  CStudio& stagedStudio) {
    std::unordered_map<std::string, BuildState> buildState;
    std::unordered_map<std::string, CPrimitive*> builtObjects;
    std::unordered_map<std::string, std::unique_ptr<CPrimitive>> ownedObjects;

    std::function<CPrimitive*(const std::string&)> buildObject =
        [&](const std::string& id) -> CPrimitive* {
            auto built = builtObjects.find(id);
            if (built != builtObjects.end()) {
                return built->second;
            }

            auto indexIt = indexById.find(id);
            if (indexIt == indexById.end()) {
                std::cerr << "Error: object '" << id << "' was referenced but not defined.\n";
                return nullptr;
            }

            BuildState& state = buildState[id];
            if (state == BuildState::Visiting) {
                std::cerr << "Error: cyclic object hierarchy involving '" << id << "'.\n";
                return nullptr;
            }
            if (state == BuildState::Done) {
                auto finished = builtObjects.find(id);
                return finished == builtObjects.end() ? nullptr : finished->second;
            }

            state = BuildState::Visiting;
            const ObjectSpec& spec = orderedSpecs[indexIt->second];
            std::unique_ptr<CPrimitive> object;

            auto resolveTexture = [&](const std::string& textureName) -> CTexture* {
                auto tex = textureMap.find(textureName);
                if (tex == textureMap.end()) {
                    std::cerr << "Error: texture '" << textureName
                              << "' not found for object '" << spec.id << "'.\n";
                    return nullptr;
                }
                return tex->second;
            };

            if (spec.type == "plane") {
                CTexture* texture = resolveTexture(spec.textureName);
                if (texture == nullptr) {
                    return nullptr;
                }
                object.reset(new CPlane(spec.point, spec.normal, texture, nullptr,
                                        spec.flipInside));
            } else if (spec.type == "sphere") {
                CTexture* texture = resolveTexture(spec.textureName);
                if (texture == nullptr) {
                    return nullptr;
                }
                object.reset(new CSphere(spec.radius, spec.center, texture, nullptr,
                                         spec.flipInside));
            } else if (spec.type == "box") {
                CTexture* texture = resolveTexture(spec.textureName);
                if (texture == nullptr) {
                    return nullptr;
                }
                object.reset(new CBox(spec.max, spec.min, texture, nullptr,
                                      spec.flipInside));
            } else if (spec.type == "cone") {
                CTexture* texture = resolveTexture(spec.textureName);
                if (texture == nullptr) {
                    return nullptr;
                }
                object.reset(new CCone(spec.base, spec.apex, spec.radius, texture, nullptr,
                                       spec.flipInside));
            } else if (spec.type == "group") {
                object.reset(new CGroup(nullptr));
            } else if (spec.type == "intersect") {
                object.reset(new CIntersection(spec.flipInside, nullptr));
            } else if (spec.type == "union") {
                object.reset(new CUnion(spec.flipInside, nullptr));
            } else {
                std::cerr << "Error: unsupported object type '" << spec.type << "'.\n";
                return nullptr;
            }

            CPrimitive* rawObject = object.get();
            builtObjects[id] = rawObject;
            ownedObjects[id] = std::move(object);

            if (spec.type == "group" || spec.type == "intersect" || spec.type == "union") {
                CGroup* group = static_cast<CGroup*>(rawObject);
                for (const std::string& childId : spec.children) {
                    if (buildObject(childId) == nullptr) {
                        return nullptr;
                    }
                    auto ownedChild = ownedObjects.find(childId);
                    if (ownedChild == ownedObjects.end() || ownedChild->second == nullptr) {
                        std::cerr << "Error: object '" << childId
                                  << "' could not be attached under '" << spec.id << "'.\n";
                        return nullptr;
                    }
                    group->Objects.push_back(ownedChild->second.release());
                }
            }

            applyTransforms(*rawObject, spec.transforms);
            state = BuildState::Done;
            return rawObject;
        };

    for (const ObjectSpec& spec : orderedSpecs) {
        if (buildObject(spec.id) == nullptr) {
            return false;
        }
    }

    for (const ObjectSpec& spec : orderedSpecs) {
        if (childUseCount.find(spec.id) != childUseCount.end()) {
            continue;
        }
        auto owned = ownedObjects.find(spec.id);
        if (owned == ownedObjects.end() || owned->second == nullptr) {
            std::cerr << "Error: top-level object '" << spec.id
                      << "' is missing during scene assembly.\n";
            return false;
        }
        stagedStudio.Objects.Objects.push_back(owned->second.release());
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

    ryml::Tree tree = ryml::parse_in_place(c4::to_csubstr(filename), c4::to_substr(data));
    auto root = tree.rootref();
    if (!validateKnownKeys(root, {"globals", "textures", "objects", "lights", "output", "camera"},
                           "scene root")) {
        return false;
    }

    CStudio stagedStudio(studio.RecurseDepth);
    stagedStudio.RecurseDepth = studio.RecurseDepth;
    stagedStudio.Oversampling = studio.Oversampling;
    stagedStudio.BackgroundColor = studio.BackgroundColor;

    if (root.has_child("globals")) {
        auto gl = root["globals"];
        if (!validateKnownKeys(gl, {"recursion_depth", "oversampling", "background_color"},
                               "globals")) {
            return false;
        }

        int recursionDepth = stagedStudio.RecurseDepth;
        if (!readOptionalScalar(gl, "recursion_depth", recursionDepth, "globals")) {
            return false;
        }
        if (recursionDepth < 0) {
            std::cerr << "Error: recursion_depth must be >= 0. Got "
                      << recursionDepth << ".\n";
            return false;
        }
        stagedStudio.RecurseDepth = recursionDepth;

        int oversampling = stagedStudio.Oversampling;
        if (!readOptionalScalar(gl, "oversampling", oversampling, "globals")) {
            return false;
        }
        if (oversampling < 1) {
            std::cerr << "Error: oversampling must be >= 1. Got "
                      << oversampling << ".\n";
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

    const bool hasCustomSceneContent =
        root.has_child("textures") || root.has_child("objects") || root.has_child("lights");

    std::unordered_map<std::string, CTexture*> textureMap;
    if (hasCustomSceneContent && root.has_child("textures")) {
        std::vector<TextureSpec> textureSpecs;
        std::unordered_map<std::string, std::size_t> textureIndexByName;
        if (!parseTextureSpecs(root["textures"], textureSpecs, textureIndexByName)) {
            return false;
        }
        if (!buildTextures(textureSpecs, textureIndexByName, stagedStudio, textureMap)) {
            return false;
        }
    }

    if (!hasCustomSceneContent) {
        SetupStudio(stagedStudio);
    } else if (root.has_child("objects")) {
        std::vector<ObjectSpec> objectSpecs;
        std::unordered_map<std::string, std::size_t> objectIndexById;
        std::unordered_map<std::string, std::size_t> childUseCount;
        if (!parseObjectSpecs(root["objects"], objectSpecs, objectIndexById, childUseCount)) {
            return false;
        }
        if (!buildObjects(objectSpecs, objectIndexById, childUseCount, textureMap, stagedStudio)) {
            return false;
        }
    }

    if (hasCustomSceneContent && root.has_child("lights")) {
        auto lights = root["lights"];
        if (!requireSeqNode(lights, "lights")) {
            return false;
        }
        for (auto light : lights) {
            if (!requireMapNode(light, "light")) {
                return false;
            }

            std::string type;
            if (!readRequiredScalar(light, "type", type, "light")) {
                return false;
            }
            type = toLowerAscii(type);

            if (type == "ambient") {
                if (!validateKnownKeys(light, {"id", "type", "intensity"}, "ambient light")) {
                    return false;
                }

                CColor intensity(0, 0, 0);
                if (!readRequiredColor(light, "intensity", intensity, "ambient light")) {
                    return false;
                }

                stagedStudio.Lights.emplace_back(new CAmbientLight(intensity));
            } else if (type == "point") {
                if (!validateKnownKeys(light, {"id", "type", "position", "intensity"},
                                       "point light")) {
                    return false;
                }

                CVector position(0, 0, 0);
                CColor intensity(0, 0, 0);
                if (!readRequiredVector(light, "position", position, "point light")) {
                    return false;
                }
                if (!readRequiredColor(light, "intensity", intensity, "point light")) {
                    return false;
                }

                stagedStudio.Lights.emplace_back(new CPointLight(position, intensity));
            } else {
                std::cerr << "Error: unsupported light type '" << type << "'.\n";
                return false;
            }
        }
    } else if (hasCustomSceneContent && root.has_child("objects")) {
        SetupLights(stagedStudio);
    }

    std::string out_fname = "output.png";
    int width = 200;
    int height = 160;
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
            fmt = toLowerAscii(fmt);
            if (fmt == "png") {
                imgType = CStorage::PNG;
            } else if (fmt == "jpg" || fmt == "jpeg") {
                imgType = CStorage::JPG;
            } else if (fmt == "tga") {
                imgType = CStorage::TGA;
            } else if (fmt == "hdr") {
                imgType = CStorage::HDR;
            } else {
                std::cerr << "Error: unsupported output format '" << fmt << "'.\n";
                return false;
            }
        }

        if (out.has_child("resolution")) {
            auto res = out["resolution"];
            if (!res.is_seq() || res.num_children() != 2) {
                std::cerr << "Error: output resolution is not a valid 2-element sequence.\n";
                return false;
            }
            if (!res[0].readable() || !res[1].readable()) {
                std::cerr << "Error: output resolution values are not readable.\n";
                return false;
            }

            int requestedWidth = 0;
            int requestedHeight = 0;
            res[0] >> requestedWidth;
            res[1] >> requestedHeight;

            if (requestedWidth <= 0 || requestedHeight <= 0) {
                std::cerr << "Error: output resolution must be positive. Got ["
                          << requestedWidth << ", " << requestedHeight << "].\n";
                return false;
            }

            constexpr int kMaxScoord = std::numeric_limits<scoord>::max();
            if (requestedWidth > kMaxScoord || requestedHeight > kMaxScoord) {
                std::cerr << "Error: output resolution exceeds max supported size ("
                          << kMaxScoord << "). Got ["
                          << requestedWidth << ", " << requestedHeight << "].\n";
                return false;
            }

            width = requestedWidth;
            height = requestedHeight;
        }
    }

    bool hasCamera = false;
    vector position(0, 0, 0);
    vector look_at(0, 0, 1);
    vector up(0, 1, 0);
    double focalLength = 1.0;
    double fov_h = 40.0;
    double fov_v = 40.0;
    if (root.has_child("camera")) {
        hasCamera = true;
        auto cam = root["camera"];
        if (!validateKnownKeys(cam, {"type", "position", "look_at", "up", "focal_length"}, "camera")) {
            return false;
        }

        std::string camType;
        if (!readOptionalScalar(cam, "type", camType, "camera")) {
            return false;
        }
        if (!camType.empty() && toLowerAscii(camType) != "perspective") {
            std::cerr << "Error: unsupported camera type '" << camType << "'.\n";
            return false;
        }

        if (cam.has_child("position") && !readVectorNode(cam["position"], position)) {
            return false;
        }
        if (cam.has_child("look_at") && !readVectorNode(cam["look_at"], look_at)) {
            return false;
        }
        if (cam.has_child("up") && !readVectorNode(cam["up"], up)) {
            return false;
        }
        if (!readRequiredScalar(cam, "focal_length", focalLength, "camera")) {
            return false;
        }
        if (focalLength <= 0.0) {
            std::cerr << "Error: camera focal_length must be > 0. Got "
                      << focalLength << ".\n";
            return false;
        }

        if ((look_at - position).NormOf() <= 0.0) {
            std::cerr << "Error: camera look_at must differ from position.\n";
            return false;
        }
        if (up.NormOf() <= 0.0) {
            std::cerr << "Error: camera up vector must be non-zero.\n";
            return false;
        }

        const double aspect = static_cast<double>(width) / static_cast<double>(height);
        fov_h = std::atan(aspect / focalLength) * 180.0 / M_PI;
        fov_v = std::atan(1.0 / focalLength) * 180.0 / M_PI;
    }

    studio.Lights.swap(stagedStudio.Lights);
    studio.Textures.swap(stagedStudio.Textures);
    studio.Objects.Objects.swap(stagedStudio.Objects.Objects);
    studio.BackgroundColor = stagedStudio.BackgroundColor;
    studio.RecurseDepth = stagedStudio.RecurseDepth;
    studio.Oversampling = stagedStudio.Oversampling;

    if (hasCamera) {
        CLine rayDir(look_at - position, position);
        COptic optic(rayDir, up, fov_h, fov_v);
        std::vector<std::unique_ptr<CCamera>> stagedCameras;
        stagedCameras.emplace_back(
            new CCamera(optic, &studio, out_fname, imgType, width, height));
        cameras.swap(stagedCameras);
    }

    return true;
}
