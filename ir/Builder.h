#pragma once

#include <mimalloc.h>
#include "ir/BasicBlock.h"
#include "ir/RegisterAllocator.h"


namespace Nyx::ir {
    class Builder {
    public:
        ~Builder();
        void switch_block(BasicBlock *block);
        Register create_load_imm_int(const int64_t &value);

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



    private:
        BasicBlock *allocate_block();

        std::vector<BasicBlock *> m_blocks;
        BasicBlock *m_curr_block;
        RegisterAllocator m_register_allocator;
        mi_heap_t *m_heap;
    };
} // namespace Nyx::ir
