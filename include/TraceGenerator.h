#ifndef TRACE_GENERATOR_H
#define TRACE_GENERATOR_H

#include <unordered_map>
#include "Node.h"
#include "json.hpp"

extern std::vector<json> trace;
void simulateExecution(const Node& node, std::unordered_map<std::string, int>& vars);

#endif // TRACE_GENERATOR_H 