#pragma once

#include <concepts>
#include <mimalloc.h>

#include <vector>
#include "Instruction.h"

namespace Nyx::ir {
    class BasicBlock {
    public:
        BasicBlock(mi_heap_t *heap, std::size_t id) : m_heap(heap), m_id(id) {}

        template<std::derived_from<Instruction> Instr, typename... Args>
        void push(Args &&...args) {
            // TODO: throw error if block is already terminated.
            void *mem = mi_heap_malloc(m_heap, sizeof(Instr));
            Instruction *instr = new (mem) Instr(std::forward<Args>(args)...);
            m_instructions.push_back(instr);
            m_code_size += instr->length();
        }

        inline void set_code_offset(size_t offset) { m_code_offset = offset; }
        inline size_t code_offset() const { return m_code_offset; }

        inline size_t code_size() const { return m_code_size; }
        inline std::size_t id() const { return m_id; }

        inline Instruction *end() { return m_instructions.back(); }

        void lower(InstructionEmitter &emitter);

    private:
        mi_heap_t *m_heap;
        std::size_t m_id;
        std::size_t m_code_size{0};
        std::size_t m_code_offset{0};
        // TODO: fix leaking memory from vector buffers.
        std::vector<Instruction *> m_instructions;
        std::vector<BasicBlock *> m_predecessors;
        std::vector<BasicBlock *> m_successors;
    };
} // namespace Nyx::ir
