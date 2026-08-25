#pragma once

#include <concepts>
#include <mimalloc.h>

#include <vector>
#include "Instruction.h"

namespace Nyx::ir {
    struct Edge {
        size_t weight;
        BasicBlock *to;

        Edge(size_t weight, BasicBlock *to) : weight(weight), to(to) {}

        static constexpr size_t CondJumpWeight = 20;
    };

    class BasicBlock {
    public:
        BasicBlock(mi_heap_t *heap, std::size_t id) : m_heap(heap), m_id(id) {}

        template<std::derived_from<Instruction> Instr, typename... Args>
        void push(Args &&...args) {
            // TODO: throw error if block is already terminated.
            void *mem = mi_heap_malloc(m_heap, sizeof(Instr));
            Instruction *instr = new (mem) Instr(std::forward<Args>(args)...);
            m_instructions.push_back(instr);
            m_stack_effect += instr->stack_cost();
            m_code_size += instr->length();
        }

        inline void set_code_offset(size_t offset) { m_code_offset = offset; }
        inline size_t code_offset() const { return m_code_offset; }

        inline size_t code_size() const { return m_code_size; }
        inline std::size_t id() const { return m_id; }

        void add_successor(Edge edge) {
            m_successors.push_back(edge);
            // block->add_predecessor(this);
        }

        void add_predecessor(BasicBlock *block) { m_predecessors.push_back(block); }

        inline std::vector<Edge> &successors() { return m_successors; }
        inline std::vector<BasicBlock *> &predecessors() { return m_predecessors; }

        inline Instruction *end() { return m_instructions.back(); }
        inline size_t stack_effect() const { return m_stack_effect; }


        void lower(InstructionEmitter &emitter);

    private:
        mi_heap_t *m_heap;
        std::size_t m_id;
        std::size_t m_code_size{0};
        std::size_t m_code_offset{0};
        std::size_t m_stack_effect{0};


        std::vector<Instruction *> m_instructions;
        std::vector<BasicBlock *> m_predecessors;
        std::vector<Edge> m_successors;
    };
} // namespace Nyx::ir
