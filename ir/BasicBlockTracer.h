#pragma once


#include <vector>
#include "ir/BasicBlock.h"

namespace Nyx::ir {
    class BasicBlockTracer {
    public:
        BasicBlockTracer(std::vector<BasicBlock *> &blocks) : m_blocks(blocks) {}

        std::vector<BasicBlock *> trace();

    private:
        std::vector<BasicBlock *> &m_blocks;
    };
} // namespace Nyx::ir
