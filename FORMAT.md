Scene File Format Specification

This document describes a JSON/YAML-based input format for defining scenes.
It covers all features supported by the RayTracer application, replacing src/setup.cpp.

# 1. Top-Level Sections

The scene file contains these sections:

- global
- cameras
- textures
- lights
- objects

# 2. Global

- recursion_depth: integer ≥ 0
- background_color: [r,g,b] floats in [0,1]

# 3. Output

- format: one of [PNG, TGA, HDR, JPG]
- resolution: [width:int, height:int]

# 4. Cameras

- name: unique ID, also used to set the name of the resulting image
- type: "perspective"
- position: [x, y, z]
- look_at: [x, y, z]
- up: [x, y, z]
- fov: [horiz_deg, vert_deg]

# 5. Textures

Each defines a CTexture subclass:

- name: unique ID
- type: one of [solid, blend, noise, turbulence, marble]
- parameters by type:
  - solid: color: [r, g, b]
  - blend/noise/turbulence/marble:
    - base_textures: [id1, id2]
    - scale: float
    - blend_coeffs: [a3, a2, a1, a0]

# 6. Lights

List of CLight objects:

- type: "ambient" or "point"
- intensity: [r, g, b]
- position: [x, y, z] (for point)
- no_shade: bool (default false)

# 7. Objects

Scene graph of primitives:

- id: unique string
- type: [sphere, plane, box, cone, group, union, intersect]
- texture: texture ID
- parameters by type:
  - sphere: center:[x, y, z], radius:float
  - plane: point:[x, y, z], normal:[x, y, z]
  - box: min:[x, y, z], max:[x, y, z]
  - cone: apex:[x, y, z], axis:[x, y, z], angle:float
  - group/union/intersect: children:[id, ...]
- flip_inside: bool
- transforms: ordered list of:
  - translate:[x, y, z]
  - rotate:[x, y, z] # Euler
  - scale:[x, y, z]

8. Example (YAML)
```yaml
global:
  recursion_depth: 5
  background_color: [0.2,0.55,0.85]

output:
  filename: "render.png"
  format: PNG
  resolution: [800,600]

camera:
  type: perspective
  position: [3,-10,6]
  look_at: [0,-2,4]
  up: [0,0,1]
  fov: [40,30]

textures:
  - name: white
    type: solid
    color: [1,1,1]
  - name: marble
    type: marble
    base_textures: [white,black]
    scale: 2.5
    blend_coeffs: [0.5,0,0.5,0]

lights:
  - type: ambient
    intensity: [0.15,0.15,0.15]
  - type: point
    position: [5,10,20]
    intensity: [1,1,1]

objects:
  - id: floor
    type: plane
    point: [0,0,0]
    normal: [0,0,1]
    texture: white
  - id: ball
    type: sphere
    center: [0,0,1]
    radius: 1
    texture: marble
    transforms:
      - translate: [0,0,0]
      - scale: [1,1,1]
```