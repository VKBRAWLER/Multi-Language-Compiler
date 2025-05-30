#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

struct Node {
    std::string label;
    std::vector<Node> children;
};

json nodeToJson(const Node& node);

#endif // NODE_H 