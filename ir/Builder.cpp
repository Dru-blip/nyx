#include "ir/Builder.h"
#include <algorithm>
#include <cassert>
#include <mimalloc.h>
#include <stack>
#include "ir/BasicBlock.h"
#include "ir/BasicBlockTracer.h"
#include "ir/BytecodeEmitter.h"


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

    void Builder::create_not() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Not>();
    }

    void Builder::create_neg() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Neg>();
    }

    void Builder::create_add() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Add>();
    }

    void Builder::create_sub() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Sub>();
    }


    void Builder::create_mul() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Mul>();
    }


    void Builder::create_div() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Div>();
    }


    void Builder::create_lt() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Lt>();
    }


    void Builder::create_lte() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Lte>();
    }


    void Builder::create_gt() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Gt>();
    }

    void Builder::create_gte() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Gte>();
    }


    void Builder::create_eq() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Eq>();
    }

    void Builder::create_neq() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Neq>();
    }

    void Builder::create_load_imm_int(const int64_t &value) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<LoadImmInt>(value);
    }

    void Builder::create_load_const(const uint16_t &idx) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<LoadConst>(idx);
    }

    void Builder::create_load_string(const uint16_t &idx) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<LoadString>(idx);
    }

    void Builder::create_store_local(uint8_t slot) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<StoreLocal>(slot);
    }

    void Builder::create_get_local(uint8_t slot) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<GetLocal>(slot);
    }

    void Builder::create_jmp(BasicBlock *target, const size_t weight) {
        assert(m_curr_block != nullptr);
        m_curr_block->add_successor({weight, target});
        m_curr_block->push<Jmp>(target);
    }

    void Builder::create_jmpif_true(BasicBlock *target, const size_t weight) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<JmpIfTrue>(target);
        m_curr_block->add_successor({weight, target});
    }

    void Builder::create_jmpif_false(BasicBlock *target, const size_t weight) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<JmpIfFalse>(target);
        m_curr_block->add_successor({weight, target});
    }

    void Builder::create_branch(BasicBlock *true_target, BasicBlock *false_target) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Branch>(true_target, false_target);
        m_curr_block->add_successor({Edge::CondJumpWeight + 20, true_target});
        m_curr_block->add_successor({1, false_target});
    }

    void Builder::create_ret() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Ret>();
    }

    void Builder::create_ret_nil() {
        assert(m_curr_block != nullptr);
        m_curr_block->push<RetNil>();
    }

    void Builder::create_call(const size_t arg_count) {
        assert(m_curr_block != nullptr);
        m_curr_block->push<Call>(arg_count);
    }

    uint8_t Builder::allocate_local() { return m_local_count++; }


    void Builder::calculate_stack_size() {
        size_t max_stack_size = 0;

        std::stack<BasicBlock *> block_stack;
        std::vector<bool> visited(m_blocks.size(), false);

        block_stack.push(m_blocks[0]);
        while (!block_stack.empty()) {
            BasicBlock *block = block_stack.top();
            block_stack.pop();

            max_stack_size += block->stack_effect();

            for (auto &successor: block->successors()) {
                if (visited[successor.to->id()])
                    continue;
                block_stack.push(successor.to);
                visited[successor.to->id()] = true;
            }
        }
        m_stack_size = max_stack_size;
    }

    std::vector<uint8_t> Builder::finalize() {
        calculate_stack_size();
        BasicBlockTracer tracer(m_blocks);
        auto blocks = tracer.trace();
        BytecodeEmitter bytecode_emitter(blocks);
        return std::move(bytecode_emitter.emit());
    }
} // namespace Nyx::ir
