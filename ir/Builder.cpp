#include "Builder.h"
#include <cassert>
#include <mimalloc.h>
#include "ir/BasicBlock.h"
#include "ir/Register.h"

namespace Nyx::ir {
    Builder::~Builder() { mi_heap_destroy(m_heap); }

    BasicBlock *Builder::allocate_block() {
        void *mem = mi_heap_malloc(m_heap, sizeof(BasicBlock));
        BasicBlock *block = new (mem) BasicBlock(m_blocks.size());
        m_blocks.push_back(block);
        return block;
    }

    void Builder::switch_block(BasicBlock *block) { m_curr_block = block; }


    Register Builder::create_add(const Register &lhs, const Register &rhs) {
        m_register_allocator.free(rhs);
        m_register_allocator.free(lhs);
        Register dst = m_register_allocator.allocate();

        assert(m_curr_block != nullptr);
        m_curr_block->push<Add>(lhs, rhs, dst);
        return dst;
    }

    Register Builder::create_sub(const Register &lhs, const Register &rhs) {
        m_register_allocator.free(rhs);
        m_register_allocator.free(lhs);
        Register dst = m_register_allocator.allocate();
        assert(m_curr_block != nullptr);
        m_curr_block->push<Sub>(lhs, rhs, dst);
        return dst;
    }


    Register Builder::create_mul(const Register &lhs, const Register &rhs) {
        m_register_allocator.free(rhs);
        m_register_allocator.free(lhs);
        Register dst = m_register_allocator.allocate();
        assert(m_curr_block != nullptr);
        m_curr_block->push<Mul>(lhs, rhs, dst);
        return dst;
    }


    Register Builder::create_div(const Register &lhs, const Register &rhs) {
        m_register_allocator.free(rhs);
        m_register_allocator.free(lhs);
        Register dst = m_register_allocator.allocate();
        assert(m_curr_block != nullptr);
        m_curr_block->push<Div>(lhs, rhs, dst);
        return dst;
    }


    Register Builder::create_lt(const Register &lhs, const Register &rhs) {
        m_register_allocator.free(rhs);
        m_register_allocator.free(lhs);
        Register dst = m_register_allocator.allocate();
        assert(m_curr_block != nullptr);
        m_curr_block->push<Lt>(lhs, rhs, dst);
        return dst;
    }


    Register Builder::create_lte(const Register &lhs, const Register &rhs) {
        m_register_allocator.free(rhs);
        m_register_allocator.free(lhs);
        Register dst = m_register_allocator.allocate();
        assert(m_curr_block != nullptr);
        m_curr_block->push<Lte>(lhs, rhs, dst);
        return dst;
    }


    Register Builder::create_gt(const Register &lhs, const Register &rhs) {
        m_register_allocator.free(rhs);
        m_register_allocator.free(lhs);
        Register dst = m_register_allocator.allocate();
        assert(m_curr_block != nullptr);
        m_curr_block->push<Gt>(lhs, rhs, dst);
        return dst;
    }

    Register Builder::create_gte(const Register &lhs, const Register &rhs) {
        m_register_allocator.free(rhs);
        m_register_allocator.free(lhs);
        Register dst = m_register_allocator.allocate();
        assert(m_curr_block != nullptr);
        m_curr_block->push<Gte>(lhs, rhs, dst);
        return dst;
    }


    Register Builder::create_eq(const Register &lhs, const Register &rhs) {
        m_register_allocator.free(rhs);
        m_register_allocator.free(lhs);
        Register dst = m_register_allocator.allocate();
        assert(m_curr_block != nullptr);
        m_curr_block->push<Eq>(lhs, rhs, dst);
        return dst;
    }

    Register Builder::create_neq(const Register &lhs, const Register &rhs) {
        m_register_allocator.free(rhs);
        m_register_allocator.free(lhs);
        Register dst = m_register_allocator.allocate();
        assert(m_curr_block != nullptr);
        m_curr_block->push<Neq>(lhs, rhs, dst);
        return dst;
    }

    Register Builder::create_load_imm_int(const int64_t &value) {
        Register dst = m_register_allocator.allocate();
        assert(m_curr_block != nullptr);
        m_curr_block->push<LoadImmInt>(value, dst);
        return dst;
    }
} // namespace Nyx::ir
