
#include "ir/BytecodeEmitter.h"
#include <print>
#include "ir/BasicBlock.h"


namespace Nyx::ir {
    void BasicBlock::lower(InstructionEmitter &emitter) {
        for (auto &instruction: m_instructions) {
            instruction->lower(emitter);
        }
    }

    void BytecodeEmitter::calculate_block_offsets() {
        size_t offset = 0;
        for (auto &block: m_blocks) {
            block->set_code_offset(offset);
            offset += block->code_size();
        }
    }

    std::vector<uint8_t> &BytecodeEmitter::emit() {
        calculate_block_offsets();

        for (auto &block: m_blocks) {
            block->lower(m_emitter);
        }

        return m_emitter.code();
    }

} // namespace Nyx::ir
