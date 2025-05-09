"""
RapidYAML (ryml) is used as the YAML parser for the RayTracer scene loader.

To vendor the library:
  $ git submodule add https://github.com/biojppm/rapidyaml.git third_party/rapidyaml

It is a header-only C++17 library. After adding, ensure your build includes:
  -Ithird_party/rapidyaml/include

Refer to https://github.com/biojppm/rapidyaml for usage and documentation.
"""