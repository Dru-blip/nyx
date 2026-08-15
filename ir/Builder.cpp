#include "ir/Builder.h"
#include <algorithm>
#include <cassert>
#include <mimalloc.h>
#include "ir/BasicBlock.h"
#include "ir/BytecodeEmitter.h"
#include "ir/Register.h"

namespace Nyx::ir {
    Builder::~Builder() {
        for (auto block: m_blocks) {
            block->~BasicBlock();
        }
        mi_heap_destroy(m_heap);
    }

    BasicBlock *Builder::create_block() {
        void *mem = mi_heap_malloc(m_heap, sizeof(BasicBlock));
        BasicBlock *block = new (mem) BasicBlock(m_heap, m_blocks.size());
        m_blocks.push_back(block);
        return block;
    }

    void Builder::set_insert_point(BasicBlock *block) { m_curr_block = block; }

    Register Builder::create_not(const Register &value) {
        m_register_allocator.free(value);
        Register dst = m_register_allocator.allocate();

        assert(m_curr_block != nullptr);
        m_curr_block->push<Not>(value, dst);
        return dst;
    }

    Register Builder::create_neg(const Register &value) {
        m_register_allocator.free(value);
        Register dst = m_register_allocator.allocate();

        assert(m_curr_block != nullptr);
        m_curr_block->push<Neg>(value, dst);
        return dst;
    }

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

    Register Builder::create_move(const Register &src, const Register &dst) {
        assert(m_curr_block != nullptr);
        m_register_allocator.free(src);
        m_curr_block->push<Move>(src, dst);
        return dst;
    }

    void Builder::create_jmp(BasicBlock *target) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Jmp>(target);
    }

    void Builder::create_jmpif_true(const Register &condition, BasicBlock *target) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<JmpIfTrue>(condition, target);
        patch_buffer.push_back(static_cast<BlockTerminator *>(m_curr_block->end()));
    }

    void Builder::create_jmpif_false(const Register &condition, BasicBlock *target) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<JmpIfFalse>(condition, target);
        patch_buffer.push_back(static_cast<BlockTerminator *>(m_curr_block->end()));
    }

    void Builder::create_ret(const Register &value) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Ret>(value);
    }


    std::vector<uint8_t> Builder::finalize() {
        BytecodeEmitter bytecode_emitter(m_blocks, patch_buffer);
        return std::move(bytecode_emitter.emit());
    }
} // namespace Nyx::ir
