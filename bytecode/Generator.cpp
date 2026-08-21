#include "bytecode/Generator.h"
#include <charconv>
#include <cstdint>
#include <string_view>
#include "ir/BasicBlock.h"
#include "ir/Register.h"
#include "parser/Ast.h"
#include "runtime/Object.h"
#include "runtime/Value.h"


namespace Nyx::bytecode {
    Executable *Generator::build_executable() {
        auto code = m_builder.finalize();
        return m_heap->alloc<Executable>(code, m_builder.register_count(), m_constants);
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

        ir::Register slot = m_builder.allocate_register();

        if (value != nullptr) {
            const auto result = lowerExpr(value);
            m_builder.create_move(result, slot);
        }

        const std::string_view name = m_ast.getSource(varDecl->name);
        m_scope.add_local(name, slot.slot());
    }

    void Generator::lowerWhileLoop(const Node *node) {
        const WhileLoop *whileLoop = static_cast<const WhileLoop *>(node);

        BasicBlock *test_block = m_builder.create_block();
        BasicBlock *loop_block = m_builder.create_block();
        BasicBlock *end_block = m_builder.create_block();

        m_builder.create_jmp(test_block, ir::Edge::CondJumpWeight + 20);

        m_builder.set_insert_point(test_block);
        ir::Register test = lowerExpr(whileLoop->test);
        m_builder.create_branch(test, loop_block, end_block);

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
            const auto test = lowerExpr(ifNode->test);
            BasicBlock *true_block = m_builder.create_block();
            BasicBlock *false_block = m_builder.create_block();
            // TODO: create a new conditional branch instruction , so that tracing works correctly.
            m_builder.create_branch(test, true_block, false_block);

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
            ir::Register result = lowerExpr(expr);
            m_builder.create_ret(result);
            return;
        }
    }

    ir::Register Generator::lowerExpr(const Node *node) {
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
            default: {
                abort();
            }
        }
    }

    ir::Register Generator::lowerGrouped(const Node *node) {
        const Unary *grouped = static_cast<const Unary *>(node);
        ir::Register child = lowerExpr(grouped->arg);
        return child;
    }

    ir::Register Generator::lowerNeg(const Node *node) {
        const Unary *neg = static_cast<const Unary *>(node);
        ir::Register child = lowerExpr(neg->arg);

        ir::Register reg = m_builder.create_neg(child);
        return reg;
    }

    ir::Register Generator::lowerNot(const Node *node) {
        const Unary *notNode = static_cast<const Unary *>(node);
        ir::Register child = lowerExpr(notNode->arg);

        ir::Register reg = m_builder.create_not(child);
        return reg;
    }

    ir::Register Generator::lowerAdd(const Node *node) {
        const Binary *add = static_cast<const Binary *>(node);
        ir::Register left = lowerExpr(add->left);
        ir::Register right = lowerExpr(add->right);

        ir::Register reg = m_builder.create_add(left, right);
        return reg;
    }

    ir::Register Generator::lowerSub(const Node *node) {
        const Binary *sub = static_cast<const Binary *>(node);
        ir::Register left = lowerExpr(sub->left);
        ir::Register right = lowerExpr(sub->right);

        ir::Register reg = m_builder.create_sub(left, right);
        return reg;
    }


    ir::Register Generator::lowerMul(const Node *node) {
        const Binary *mul = static_cast<const Binary *>(node);
        ir::Register left = lowerExpr(mul->left);
        ir::Register right = lowerExpr(mul->right);

        ir::Register reg = m_builder.create_mul(left, right);
        return reg;
    }

    ir::Register Generator::lowerDiv(const Node *node) {
        const Binary *div = static_cast<const Binary *>(node);
        ir::Register left = lowerExpr(div->left);
        ir::Register right = lowerExpr(div->right);

        ir::Register reg = m_builder.create_div(left, right);
        return reg;
    }

    ir::Register Generator::lowerLt(const Node *node) {
        const Binary *lt = static_cast<const Binary *>(node);
        ir::Register left = lowerExpr(lt->left);
        ir::Register right = lowerExpr(lt->right);

        ir::Register reg = m_builder.create_lt(left, right);
        return reg;
    }


    ir::Register Generator::lowerLte(const Node *node) {
        const Binary *lte = static_cast<const Binary *>(node);
        ir::Register left = lowerExpr(lte->left);
        ir::Register right = lowerExpr(lte->right);

        ir::Register reg = m_builder.create_lte(left, right);
        return reg;
    }

    ir::Register Generator::lowerGt(const Node *node) {
        const Binary *gt = static_cast<const Binary *>(node);
        ir::Register left = lowerExpr(gt->left);
        ir::Register right = lowerExpr(gt->right);

        ir::Register reg = m_builder.create_gt(left, right);
        return reg;
    }

    ir::Register Generator::lowerGte(const Node *node) {
        const Binary *gte = static_cast<const Binary *>(node);
        ir::Register left = lowerExpr(gte->left);
        ir::Register right = lowerExpr(gte->right);

        ir::Register reg = m_builder.create_gte(left, right);
        return reg;
    }

    ir::Register Generator::lowerEq(const Node *node) {
        const Binary *eq = static_cast<const Binary *>(node);
        ir::Register left = lowerExpr(eq->left);
        ir::Register right = lowerExpr(eq->right);

        ir::Register reg = m_builder.create_eq(left, right);
        return reg;
    }

    ir::Register Generator::lowerNeq(const Node *node) {
        const Binary *neq = static_cast<const Binary *>(node);
        ir::Register left = lowerExpr(neq->left);
        ir::Register right = lowerExpr(neq->right);

        ir::Register reg = m_builder.create_neq(left, right);
        return reg;
    }

    ir::Register Generator::lowerAnd(const Node *node) {
        const Binary *and_node = static_cast<const Binary *>(node);

        ir::Register left = lowerExpr(and_node->left);

        BasicBlock *true_block = m_builder.create_block();
        BasicBlock *end_block = m_builder.create_block();

        ir::Register dst = m_builder.allocate_register();

        m_builder.create_move(left, dst);
        m_builder.create_jmpif_false(dst, end_block, ir::Edge::CondJumpWeight);

        m_builder.set_insert_point(true_block);
        ir::Register right = lowerExpr(and_node->right);
        m_builder.create_move(right, dst);

        m_builder.create_jmp(end_block, ir::Edge::CondJumpWeight + 20);

        m_builder.set_insert_point(end_block);

        return dst;
    }

    ir::Register Generator::lowerOr(const Node *node) {
        const Binary *or_node = static_cast<const Binary *>(node);

        ir::Register left = lowerExpr(or_node->left);

        BasicBlock *false_block = m_builder.create_block();
        BasicBlock *end_block = m_builder.create_block();

        ir::Register dst = m_builder.allocate_register();

        // TODO: should combine both move and jump into a single instruction.
        m_builder.create_move(left, dst);
        m_builder.create_jmpif_true(dst, end_block, ir::Edge::CondJumpWeight);

        m_builder.set_insert_point(false_block);
        ir::Register right = lowerExpr(or_node->right);
        m_builder.create_move(right, dst);

        m_builder.create_jmp(end_block, ir::Edge::CondJumpWeight + 20);

        m_builder.set_insert_point(end_block);

        return dst;
    }

    ir::Register Generator::lowerAssignment(const Node *node) {
        const Binary *assign_node = static_cast<const Binary *>(node);
        ir::Register dst = lowerExpr(assign_node->left);
        ir::Register src = lowerExpr(assign_node->right);
        m_builder.create_move(src, dst);
        return dst;
    }

    ir::Register Generator::lowerIdentifier(const Node *node) {
        const auto name = m_ast.getSource(node);
        uint8_t slot = m_scope.resolve(name);
        // TODO: throw error if not found.
        return ir::Register(slot);
    }

    ir::Register Generator::lowerString(const Node *node) {
        const auto str = m_ast.getSource(node);
        const uint16_t idx = add_string_constant(str.data(), str.size());

        ir::Register reg = m_builder.create_load_string(idx);
        return reg;
    }

    ir::Register Generator::lowerInt(const Node *node) {
        const auto int_str = m_ast.getSource(node);

        int64_t value = 0;
        // handle error
        std::from_chars(int_str.data(), int_str.data() + int_str.size(), value);

        ir::Register reg = m_builder.create_load_imm_int(value);
        return reg;
    }

    uint16_t Generator::add_string_constant(const char *data, std::size_t size) {
        // TODO: should intern string.
        // TODO: check for existing string constants inside string table.
        String *str = String::create(m_heap.get(), data, size);
        const auto val = Value::from_object(str);
        m_constants.push_back(val);
        return static_cast<uint16_t>(m_constants.size() - 1);
    }
} // namespace Nyx::bytecode
