#ifndef IR_H
#define IR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

// Intermediate Representation (IR) Node
struct IRNode {
  std::string opcode;
  std::vector<std::string> operands;

  IRNode(const std::string& op, const std::vector<std::string>& ops)
    : opcode(op), operands(ops) {}
};

// IR Generator Class
class IRGenerator {
public:
  IRGenerator() = default;

  // Generate an IR instruction
  void generate(const std::string& opcode, const std::vector<std::string>& operands) {
    irNodes.emplace_back(opcode, operands);
  }

  // Get all generated IR instructions
  const std::vector<IRNode>& getIR() const {
    return irNodes;
  }

  // Print IR for debugging
  void printIR() const {
    for (const auto& node : irNodes) {
      std::cout << node.opcode;
      for (const auto& operand : node.operands) {
        std::cout << " " << operand;
      }
      std::cout << std::endl;
    }
  }

private:
  std::vector<IRNode> irNodes;
};

#endif // IR_H