
#pragma once

#include <concepts>
#include <vector>
#include "Instruction.h"

namespace Nyx::ir {
    class BasicBlock {
    public:
        BasicBlock(std::size_t id) : m_id(id) {}

        template<std::derived_from<Instruction> Instr, typename... Args>
        void push(Args... args) {
            m_instructions.push_back(Instr(args...));
        }

    private:
        std::size_t m_id;
        std::vector<Instruction> m_instructions;
        std::size_t m_code_offset{0};
        std::vector<BasicBlock *> m_predecessors;
        std::vector<BasicBlock *> m_successors;
    };
} // namespace Nyx::ir
