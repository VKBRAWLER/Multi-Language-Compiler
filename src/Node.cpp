#include "Node.h"

json nodeToJson(const Node& node) {
    json j;
    j["name"] = node.label;
    j["children"] = json::array();
    for (const auto& child : node.children) {
        j["children"].push_back(nodeToJson(child));
    }
    return j;
} 