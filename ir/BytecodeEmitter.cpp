#include "ir/BytecodeEmitter.h"
#include "ir/BasicBlock.h"


namespace Nyx::ir {
    void BasicBlock::lower(std::vector<uint8_t> &buffer) {
        for (auto &instruction: m_instructions) {
            instruction->lower(buffer);
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

        return m_emitter;
    }

} // namespace Nyx::ir
