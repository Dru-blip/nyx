#pragma once

#include <mimalloc.h>
#include "ir/BasicBlock.h"
#include "ir/Register.h"
#include "ir/RegisterAllocator.h"


namespace Nyx::ir {
    class Builder {
    public:
        Builder() { m_heap = mi_heap_new(); };
        ~Builder();

        void set_insert_point(BasicBlock *block);

        Register create_load_imm_int(const int64_t &value);
        Register create_load_const(const uint16_t &idx);
        Register create_load_string(const uint16_t &idx);
        Register create_move(const Register &src, const Register &dst);

        Register create_not(const Register &value);
        Register create_neg(const Register &value);

        Register create_add(const Register &lhs, const Register &rhs);
        Register create_sub(const Register &lhs, const Register &rhs);
        Register create_mul(const Register &lhs, const Register &rhs);
        Register create_div(const Register &lhs, const Register &rhs);

        Register create_lt(const Register &lhs, const Register &rhs);
        Register create_lte(const Register &lhs, const Register &rhs);
        Register create_gt(const Register &lhs, const Register &rhs);
        Register create_gte(const Register &lhs, const Register &rhs);

        Register create_eq(const Register &lhs, const Register &rhs);
        Register create_neq(const Register &lhs, const Register &rhs);

        Register create_call(const Register &callee, const size_t arg_count);

        void create_jmp(BasicBlock *target, const size_t weight);
        void create_jmpif_true(const Register &condition, BasicBlock *target, const size_t weight);
        void create_jmpif_false(const Register &condition, BasicBlock *target, const size_t weight);

        void create_branch(const Register &condition, BasicBlock *true_target,
                           BasicBlock *false_target);

        void create_ret(const Register &value);
        void create_ret_nil();

        BasicBlock *create_block();

        Register allocate_register() { return m_register_allocator.allocate(); }

        Register allocate_local() {return m_register_allocator.allocate_local();}

        uint8_t register_count() const { return m_register_allocator.max_used(); }

        std::vector<uint8_t> finalize();

    private:
        std::vector<BasicBlock *> m_blocks;
        BasicBlock *m_curr_block;
        RegisterAllocator m_register_allocator;
        mi_heap_t *m_heap;
    };
} // namespace Nyx::ir
