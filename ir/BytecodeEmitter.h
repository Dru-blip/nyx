
#pragma once

#include <vector>
#include "ir/BasicBlock.h"

namespace Nyx::ir {
    class BytecodeEmitter {
    public:
        BytecodeEmitter(std::vector<BasicBlock *> &blocks) :
            m_blocks(blocks) {}
        std::vector<uint8_t>& emit();

    private:
        void calculate_block_offsets();
        void patch_instructions();

        std::vector<BasicBlock *> &m_blocks;
        InstructionEmitter m_emitter;
    };
} // namespace Nyx::ir
