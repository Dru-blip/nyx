
#pragma once

#include <vector>
#include "ir/BasicBlock.h"

namespace Nyx::ir {
    class BytecodeEmitter {
    public:
        BytecodeEmitter(std::vector<BasicBlock *> &blocks,
                        std::vector<BlockTerminator *> &patch_buffer) :
            m_blocks(blocks), m_patch_buffer(patch_buffer) {}
        std::vector<uint8_t>& emit();

    private:
        void calculate_block_offsets();
        void patch_instructions();

        std::vector<BasicBlock *> &m_blocks;
        std::vector<BlockTerminator *> &m_patch_buffer;
        InstructionEmitter m_emitter;
    };
} // namespace Nyx::ir
