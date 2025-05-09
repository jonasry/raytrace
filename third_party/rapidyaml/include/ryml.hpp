#pragma once
#include <string>

// Minimal stub for RapidYAML (ryml) to allow compilation.
namespace c4 { namespace yml {
    // string view alias
    using csubstr = std::string;
}} // namespace c4::yml

namespace ryml {
    struct Tree {
        // stubbed: no real parse
        static Tree parse_in_place(const c4::yml::csubstr&) { return Tree(); }
        struct NodeRef {
            bool has_child(const char*) const { return false; }
            NodeRef operator[](const char*) const { return NodeRef(); }
            struct Val { int to_int() const { return 0; } };
            Val val() const { return Val(); }
        };
        NodeRef rootref() const { return NodeRef(); }
    };
    // free-function stubs matching typical API
    inline Tree parse_in_place(const c4::yml::csubstr& cs) { return Tree::parse_in_place(cs); }
    inline c4::yml::csubstr to_csubstr(const std::string& s) { return s; }
} // namespace ryml