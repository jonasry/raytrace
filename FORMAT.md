Scene File Format Specification

This document describes the YAML scene format supported by `SceneLoader`.

# Top-Level Sections

Supported top-level keys:

- `globals`
- `output`
- `camera`
- `textures`
- `lights`
- `objects`

# Globals

```yaml
globals:
  recursion_depth: 5      # integer >= 0
  oversampling: 1         # integer >= 1
  background_color: [0,0,0]
```

# Output

```yaml
output:
  filename: "render.png"
  format: PNG             # PNG, JPG, TGA, HDR
  resolution: [400,300]
```

# Camera

One camera is loaded per scene file.

```yaml
camera:
  type: perspective
  position: [3,-10,6]
  look_at: [0,-2,4]
  up: [0,0,1]
  fov: [20,20]
```

# Textures

Supported texture types:

- `solid`
- `material`
- `checker`
- `noise`
- `turbulence`
- `marble`

## Solid

```yaml
- name: white
  type: solid
  color: [1,1,1]
```

## Material

Maps to the full `CTexture(diffuse, specular, shine_pow, ks, ior)` constructor.
Any color field in the scene format may be written either as `[r,g,b]` or as
`to_spec(r,g,b)`, which applies the legacy `FHelper` mapping component-wise.
This is the inverse of the normal-incidence Fresnel reflectance formula,
turning reflectance values into the corresponding per-channel specular/IOR-like
values for this renderer's material model.

```yaml
- name: polished
  type: material
  diffuse: [0,0,0]
  specular: to_spec(0.80, 0.40, 0.20)
  shine_pow: 0.1
  ks: 0.6
  ior: 1.0
```

## Checker

```yaml
- name: floor_checker
  type: checker
  base_textures: [white, blue]
  cells: [2,2,2]
```

## Procedural (`noise`, `turbulence`, `marble`)

```yaml
- name: swirled
  type: marble
  base_textures: [white, black]
  scale: 0.25
```

Notes:

- `base_textures` must contain exactly two texture names.
- Texture references may point to textures defined earlier or later in the file.

# Lights

Supported light types:

- `ambient`
- `point`

```yaml
lights:
  - id: key
    type: point
    position: [5,10,20]
    intensity: [0.3,0.3,0.3]

  - id: ambient
    type: ambient
    intensity: [0.3,0.288,0.261]
```

# Objects

Supported object types:

- `plane`
- `sphere`
- `box`
- `cone`
- `group`
- `intersect`
- `union`

Every object requires:

- `id`
- `type`

Optional fields shared by object types:

- `transforms`: ordered list of `translate`, `rotate`, or `scale`
- `flip_inside`: supported by `plane`, `sphere`, `box`, `cone`, `intersect`, and `union`

## Plane

```yaml
- id: floor
  type: plane
  point: [0,0,0]
  normal: [0,0,1]
  texture: white
```

## Sphere

```yaml
- id: ball
  type: sphere
  center: [0,0,1]
  radius: 1
  texture: polished
```

## Box

```yaml
- id: block
  type: box
  min: [0,0,0]
  max: [1,1,1]
  texture: polished
```

## Cone

```yaml
- id: tube
  type: cone
  base: [0,0,0]
  apex: [0,1,0]
  radius: 1
  texture: polished
```

Note: this maps to the current runtime constructor `(base, apex, radius)` rather than an angle-based cone.

## Group / Intersect / Union

```yaml
- id: cluster
  type: group
  children: [part_a, part_b]
```

Rules:

- `children` entries must refer to other object ids in the same file.
- Each child object may belong to only one parent composite.
- Child objects are still defined in the top-level `objects` list; ownership is transferred into the parent when loaded.

## Transforms

Transforms are applied in the listed order.

```yaml
transforms:
  - translate: [0,0,1]
  - rotate: [0,0,0.3]
  - scale: [1,1,2]
```

# Full Example

See [examples/legacy_setup.yaml](/Users/luck/engine/examples/legacy_setup.yaml) for a full scene translated from the historical `setup.cpp`.
