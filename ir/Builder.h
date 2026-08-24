#pragma once

#include <mimalloc.h>
#include "ir/BasicBlock.h"


namespace Nyx::ir {
    class Builder {
    public:
        Builder() { m_heap = mi_heap_new(); };
        ~Builder();

        void set_insert_point(BasicBlock *block);

        void create_load_imm_int(const int64_t &value);
        void create_load_const(const uint16_t &idx);
        void create_load_string(const uint16_t &idx);

        void create_not();
        void create_neg();

        void create_add();
        void create_sub();
        void create_mul();
        void create_div();

        void create_lt();
        void create_lte();
        void create_gt();
        void create_gte();

        void create_eq();
        void create_neq();

        void create_call(const size_t arg_count);

        void create_jmp(BasicBlock *target, const size_t weight);
        void create_jmpif_true(BasicBlock *target, const size_t weight);
        void create_jmpif_false(BasicBlock *target, const size_t weight);

        void create_branch(BasicBlock *true_target, BasicBlock *false_target);

        void create_ret();
        void create_ret_nil();

        BasicBlock *create_block();

        uint32_t slot_count() const { return m_slot_count; }

        std::vector<uint8_t> finalize();

    private:
        std::vector<BasicBlock *> m_blocks;
        BasicBlock *m_curr_block;
        mi_heap_t *m_heap;
        uint32_t m_slot_count{0};
    };
} // namespace Nyx::ir
