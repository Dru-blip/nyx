#include "bytecode/Generator.h"
#include <charconv>
#include <cstdint>
#include <string_view>
#include "ir/BasicBlock.h"
#include "parser/Ast.h"
#include "runtime/VM.h"
#include "runtime/Value.h"

namespace Nyx::bytecode {
    Executable *Generator::build_executable() {
        auto code = m_builder.finalize();
        return m_vm.heap()->alloc<Executable>(code, m_constants, m_builder.local_count(),
                                              m_builder.stack_size());
    }

    Executable *Generator::compile() {
        BasicBlock *entry = m_builder.create_block();
        m_builder.set_insert_point(entry);

        for (const Node *root: m_ast.roots()) {
            lowerRoot(root);
        }

        m_builder.create_ret_nil();
        return build_executable();
    }


    void Generator::lowerRoot(const Node *node) {
        switch (node->tag) {
            case NodeTag::VarDecl: {
                return lowerVarDecl(node);
            }
            case NodeTag::While: {
                return lowerWhileLoop(node);
            }
            case NodeTag::Loop: {
                return lowerLoop(node);
            }
            case NodeTag::Break: {
                return lowerBreak(node);
            }
            case NodeTag::Continue: {
                return lowerContinue(node);
            }
            case NodeTag::If: {
                return lowerIf(node);
            }
            case NodeTag::BlockStmt: {
                return lowerBlockStmt(node);
            }
            case NodeTag::Ret: {
                return lowerRet(node);
            }
            case NodeTag::ExprStmt: {
                return lowerExprStmt(node);
            }
            default: {
                abort();
            }
        }
    }

    void Generator::lowerVarDecl(const Node *node) {
        const VarDecl *varDecl = static_cast<const VarDecl *>(node);
        const Node *value = varDecl->initializer;

        uint8_t slot = m_builder.allocate_local();

        if (value != nullptr) {
            lowerExpr(value);
            // TODO: should emit store local instruction or
            // move the top value to the slot.
            m_builder.create_store_local(slot);
        }

        const std::string_view name = m_ast.getSource(varDecl->name);
        m_scope.add_local(name, slot);
    }

    void Generator::lowerWhileLoop(const Node *node) {
        const WhileLoop *whileLoop = static_cast<const WhileLoop *>(node);

        BasicBlock *test_block = m_builder.create_block();
        BasicBlock *loop_block = m_builder.create_block();
        BasicBlock *end_block = m_builder.create_block();

        m_builder.create_jmp(test_block, ir::Edge::CondJumpWeight + 20);

        m_builder.set_insert_point(test_block);
        lowerExpr(whileLoop->test);
        m_builder.create_branch(loop_block, end_block);

        m_builder.set_insert_point(loop_block);
        lowerRoot(whileLoop->body);
        m_builder.create_jmp(test_block, ir::Edge::CondJumpWeight + 20);

        m_builder.set_insert_point(end_block);
    }

    void Generator::lowerLoop(const Node *node) {
        const Loop *loop = static_cast<const Loop *>(node);

        BasicBlock *loop_block = m_builder.create_block();
        BasicBlock *end_block = m_builder.create_block();

        m_loop_stack.push_back({loop_block, end_block});

        m_builder.create_jmp(loop_block, ir::Edge::CondJumpWeight + 20);
        m_builder.set_insert_point(loop_block);
        lowerRoot(loop->body);
        m_builder.create_jmp(loop_block, ir::Edge::CondJumpWeight + 20);
        m_builder.set_insert_point(end_block);

        m_loop_stack.pop_back();
    }

    void Generator::lowerBreak(const Node *node) {
        (void) node;
        if (m_loop_stack.empty()) {
            // TODO: raise error , break outside of loop.
            return;
        }
        const auto [loop_block, end_block] = m_loop_stack.back();
        m_builder.create_jmp(end_block, ir::Edge::CondJumpWeight + 10);
    }

    void Generator::lowerContinue(const Node *node) {
        (void) node;
        if (m_loop_stack.empty()) {
            // TODO: raise error , continue outside of loop.
            return;
        }
        const auto [loop_block, end_block] = m_loop_stack.back();
        m_builder.create_jmp(loop_block, ir::Edge::CondJumpWeight + 20);
    }

    void Generator::lowerIf(const Node *node) {
        Node *current = const_cast<Node *>(node);
        BasicBlock *end_block = m_builder.create_block();
        while (current != nullptr) {
            if (current->tag != NodeTag::If)
                break;
            const If *ifNode = static_cast<const If *>(current);
            lowerExpr(ifNode->test);
            BasicBlock *true_block = m_builder.create_block();
            BasicBlock *false_block = m_builder.create_block();
            // TODO: create a new conditional branch instruction , so that tracing works correctly.
            m_builder.create_branch(true_block, false_block);

            m_builder.set_insert_point(true_block);
            lowerRoot(ifNode->consequent);
            m_builder.create_jmp(end_block, ir::Edge::CondJumpWeight + 4);

            m_builder.set_insert_point(false_block);
            current = ifNode->alternate;
        }

        if (current) {
            lowerRoot(current);
            m_builder.create_jmp(end_block, ir::Edge::CondJumpWeight);
        } else {
            m_builder.create_jmp(end_block, ir::Edge::CondJumpWeight);
        }

        m_builder.set_insert_point(end_block);
    }

    void Generator::lowerBlockStmt(const Node *node) {
        const BlockStmt *blockStmt = static_cast<const BlockStmt *>(node);

        for (const Node *stmt: blockStmt->stmts) {
            lowerRoot(stmt);
        }
    }

    void Generator::lowerExprStmt(const Node *node) {
        const ExprStmt *exprStmt = static_cast<const ExprStmt *>(node);
        lowerExpr(exprStmt->expr);
    }

    void Generator::lowerRet(const Node *node) {
        const Return *ret = static_cast<const Return *>(node);

        if (ret->value.has_value()) {
            const Node *expr = *ret->value;
            lowerExpr(expr);
            m_builder.create_ret();
            return;
        }
    }

    void Generator::lowerExpr(const Node *node) {
        switch (node->tag) {
            case NodeTag::Integer: {
                return lowerInt(node);
            }
            case NodeTag::String: {
                return lowerString(node);
            }
            case NodeTag::Identifier: {
                return lowerIdentifier(node);
            }
            case NodeTag::Not: {
                return lowerNot(node);
            }
            case NodeTag::Grouped: {
                return lowerGrouped(node);
            }
            case NodeTag::Add: {
                return lowerAdd(node);
            }
            case NodeTag::Sub: {
                return lowerSub(node);
            }
            case NodeTag::Mul: {
                return lowerMul(node);
            }
            case NodeTag::Div: {
                return lowerDiv(node);
            }
            case NodeTag::Neg: {
                return lowerNeg(node);
            }
            case NodeTag::Lt: {
                return lowerLt(node);
            }
            case NodeTag::Lte: {
                return lowerLte(node);
            }
            case NodeTag::Gt: {
                return lowerGt(node);
            }
            case NodeTag::Gte: {
                return lowerGte(node);
            }
            case NodeTag::Eq: {
                return lowerEq(node);
            }
            case NodeTag::Neq: {
                return lowerNeq(node);
            }
            case NodeTag::And: {
                return lowerAnd(node);
            }
            case NodeTag::Or: {
                return lowerOr(node);
            }
            case NodeTag::Assignment: {
                return lowerAssignment(node);
            }
            case NodeTag::Call: {
                return lowerCall(node);
            }
            default: {
                abort();
            }
        }
    }

    void Generator::lowerCall(const Node *node) {
        const CallExpr *call = static_cast<const CallExpr *>(node);
        lowerExpr(call->callee);
        // Calle is at the top of the stack.

        for (const Node *arg: call->args) {
            lowerExpr(arg);
        }

        m_builder.create_call(call->args.size());
    }

    void Generator::lowerGrouped(const Node *node) {
        const Unary *grouped = static_cast<const Unary *>(node);
        lowerExpr(grouped->arg);
    }

    void Generator::lowerNeg(const Node *node) {
        const Unary *neg = static_cast<const Unary *>(node);
        lowerExpr(neg->arg);

        m_builder.create_neg();
    }

    void Generator::lowerNot(const Node *node) {
        const Unary *notNode = static_cast<const Unary *>(node);
        lowerExpr(notNode->arg);

        m_builder.create_not();
    }

    void Generator::lowerAdd(const Node *node) {
        const Binary *add = static_cast<const Binary *>(node);
        lowerExpr(add->left);
        lowerExpr(add->right);

        m_builder.create_add();
    }

    void Generator::lowerSub(const Node *node) {
        const Binary *sub = static_cast<const Binary *>(node);
        lowerExpr(sub->left);
        lowerExpr(sub->right);

        m_builder.create_sub();
    }


    void Generator::lowerMul(const Node *node) {
        const Binary *mul = static_cast<const Binary *>(node);
        lowerExpr(mul->left);
        lowerExpr(mul->right);

        m_builder.create_mul();
    }

    void Generator::lowerDiv(const Node *node) {
        const Binary *div = static_cast<const Binary *>(node);
        lowerExpr(div->left);
        lowerExpr(div->right);

        m_builder.create_div();
    }

    void Generator::lowerLt(const Node *node) {
        const Binary *lt = static_cast<const Binary *>(node);
        lowerExpr(lt->left);
        lowerExpr(lt->right);

        m_builder.create_lt();
    }


    void Generator::lowerLte(const Node *node) {
        const Binary *lte = static_cast<const Binary *>(node);
        lowerExpr(lte->left);
        lowerExpr(lte->right);

        m_builder.create_lte();
    }

    void Generator::lowerGt(const Node *node) {
        const Binary *gt = static_cast<const Binary *>(node);
        lowerExpr(gt->left);
        lowerExpr(gt->right);

        m_builder.create_gt();
    }

    void Generator::lowerGte(const Node *node) {
        const Binary *gte = static_cast<const Binary *>(node);
        lowerExpr(gte->left);
        lowerExpr(gte->right);

        m_builder.create_gte();
    }

    void Generator::lowerEq(const Node *node) {
        const Binary *eq = static_cast<const Binary *>(node);
        lowerExpr(eq->left);
        lowerExpr(eq->right);

        m_builder.create_eq();
    }

    void Generator::lowerNeq(const Node *node) {
        const Binary *neq = static_cast<const Binary *>(node);
        lowerExpr(neq->left);
        lowerExpr(neq->right);

        m_builder.create_neq();
    }

    void Generator::lowerAnd(const Node *node) {
        const Binary *and_node = static_cast<const Binary *>(node);

        lowerExpr(and_node->left);

        // TODO:
        //  BasicBlock *true_block = m_builder.create_block();
        //  BasicBlock *end_block = m_builder.create_block();

        // ir::Register dst = m_builder.allocate_register();

        // m_builder.create_move(left, dst);
        // m_builder.create_jmpif_false(dst, end_block, ir::Edge::CondJumpWeight);

        // m_builder.set_insert_point(true_block);
        // ir::Register right = lowerExpr(and_node->right);
        // m_builder.create_move(right, dst);

        // m_builder.create_jmp(end_block, ir::Edge::CondJumpWeight + 20);

        // m_builder.set_insert_point(end_block);

        // return dst;
    }

    void Generator::lowerOr(const Node *node) {
        const Binary *or_node = static_cast<const Binary *>(node);

        lowerExpr(or_node->left);

        // TODO: implement short-circuit
        //  BasicBlock *false_block = m_builder.create_block();
        //  BasicBlock *end_block = m_builder.create_block();

        // ir::Register dst = m_builder.allocate_register();

        // // TODO: should combine both move and jump into a single instruction.
        // m_builder.create_move(left, dst);
        // m_builder.create_jmpif_true(dst, end_block, ir::Edge::CondJumpWeight);

        // m_builder.set_insert_point(false_block);
        // ir::Register right = lowerExpr(or_node->right);
        // m_builder.create_move(right, dst);

        // m_builder.create_jmp(end_block, ir::Edge::CondJumpWeight + 20);

        // m_builder.set_insert_point(end_block);

        // return dst;
    }

    void Generator::lowerAssignment(const Node *node) {
        // TODO: should emit a store instruction.
        const Binary *assign_node = static_cast<const Binary *>(node);
        lowerExpr(assign_node->left);
        lowerExpr(assign_node->right);
    }

    void Generator::lowerIdentifier(const Node *node) {
        // TODO: should emit a load or just access the slot directly if reference is local or load
        // module or global context.
        const auto name = m_ast.getSource(node);
        const uint8_t slot = m_scope.resolve(name);
        // TODO: throw error if not found.
        m_builder.create_get_local(slot);
    }

    void Generator::lowerString(const Node *node) {
        const auto str = m_ast.getSource(node);
        const uint16_t idx = add_string_constant(str.data(), str.size());

        m_builder.create_load_const(idx);
    }

    void Generator::lowerInt(const Node *node) {
        const auto int_str = m_ast.getSource(node);

        int64_t value = 0;
        // handle error
        std::from_chars(int_str.data(), int_str.data() + int_str.size(), value);

        const uint16_t idx = add_int_constant(value);

        m_builder.create_load_const(idx);
    }

    uint16_t Generator::add_string_constant(const char *data, std::size_t size) {
        // TODO: should intern string.
        // TODO: check for existing string constants inside string table.
        String *str = String::create(m_vm, data, size);
        const auto val = Value::from_object(str);
        m_constants.push_back(val);
        return static_cast<uint16_t>(m_constants.size() - 1);
    }

    uint16_t Generator::add_int_constant(int64_t value) {
        const auto val = Value(value);
        m_constants.push_back(val);
        return static_cast<uint16_t>(m_constants.size() - 1);
    }
} // namespace Nyx::bytecode
