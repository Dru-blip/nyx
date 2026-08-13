#include "bytecode/Generator.h"
#include <charconv>
#include <cstdint>
#include <string_view>
#include "bytecode/Instruction.h"


namespace Nyx::bytecode {
    Executable *Generator::build_executable() {
        // return m_heap->alloc<Executable>(m_emitter.code(), m_constants);
        return nullptr;
    }

    Executable *Generator::compile() {
        for (const Node *root: m_ast.roots()) {
            lowerRoot(root);
        }
        return build_executable();
    }


    void Generator::lowerRoot(const Node *node) {
        switch (node->tag) {
            case NodeTag::Ret: {
                lowerRet(node);
            }
            default: {
            }
        }
    }

    void Generator::lowerRet(const Node *node) {
        const Return *ret = static_cast<const Return *>(node);

        if (ret->value.has_value()) {
            const Node *expr = *ret->value;
            Register result = lowerExpr(expr);
            m_builder.create_ret(result);
            return;
        }
    }

    Register Generator::lowerExpr(const Node *node) {
        switch (node->tag) {
            case NodeTag::Integer: {
                return lowerInt(node);
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
            default: {
                abort();
            }
        }
    }

    Register Generator::lowerGrouped(const Node *node) {
        const Unary *grouped = static_cast<const Unary *>(node);
        Register child = lowerExpr(grouped->arg);
        return child;
    }

    Register Generator::lowerNeg(const Node *node) {
        const Unary *neg = static_cast<const Unary *>(node);
        Register child = lowerExpr(neg->arg);

        Register reg = m_builder.create_neg(child);
        return reg;
    }

    Register Generator::lowerNot(const Node *node) {
        const Unary *notNode = static_cast<const Unary *>(node);
        Register child = lowerExpr(notNode->arg);

        Register reg = m_builder.create_not(child);
        return reg;
    }

    Register Generator::lowerAdd(const Node *node) {
        const Binary *add = static_cast<const Binary *>(node);
        Register left = lowerExpr(add->left);
        Register right = lowerExpr(add->right);

        Register reg = m_builder.create_add(left, right);
        return reg;
    }

    Register Generator::lowerSub(const Node *node) {
        const Binary *sub = static_cast<const Binary *>(node);
        Register left = lowerExpr(sub->left);
        Register right = lowerExpr(sub->right);

        Register reg = m_builder.create_sub(left, right);
        return reg;
    }


    Register Generator::lowerMul(const Node *node) {
        const Binary *mul = static_cast<const Binary *>(node);
        Register left = lowerExpr(mul->left);
        Register right = lowerExpr(mul->right);

        Register reg = m_builder.create_mul(left, right);
        return reg;
    }

    Register Generator::lowerDiv(const Node *node) {
        const Binary *div = static_cast<const Binary *>(node);
        Register left = lowerExpr(div->left);
        Register right = lowerExpr(div->right);

        Register reg = m_builder.create_div(left, right);
        return reg;
    }

    Register Generator::lowerLt(const Node *node) {
        const Binary *lt = static_cast<const Binary *>(node);
        Register left = lowerExpr(lt->left);
        Register right = lowerExpr(lt->right);

        Register reg = m_builder.create_lt(left, right);
        return reg;
    }


    Register Generator::lowerLte(const Node *node) {
        const Binary *lte = static_cast<const Binary *>(node);
        Register left = lowerExpr(lte->left);
        Register right = lowerExpr(lte->right);

        Register reg = m_builder.create_lte(left, right);
        return reg;
    }

    Register Generator::lowerGt(const Node *node) {
        const Binary *gt = static_cast<const Binary *>(node);
        Register left = lowerExpr(gt->left);
        Register right = lowerExpr(gt->right);

        Register reg = m_builder.create_gt(left, right);
        return reg;
    }

    Register Generator::lowerGte(const Node *node) {
        const Binary *gte = static_cast<const Binary *>(node);
        Register left = lowerExpr(gte->left);
        Register right = lowerExpr(gte->right);

        Register reg = m_builder.create_gte(left, right);
        return reg;
    }

    Register Generator::lowerEq(const Node *node) {
        const Binary *eq = static_cast<const Binary *>(node);
        Register left = lowerExpr(eq->left);
        Register right = lowerExpr(eq->right);

        Register reg = m_builder.create_eq(left, right);
        return reg;
    }

    Register Generator::lowerNeq(const Node *node) {
        const Binary *neq = static_cast<const Binary *>(node);
        Register left = lowerExpr(neq->left);
        Register right = lowerExpr(neq->right);

        Register reg = m_builder.create_neq(left, right);
        return reg;
    }

    Register Generator::lowerAnd(const Node *node) {
        const Binary *and_node = static_cast<const Binary *>(node);
        Register left = lowerExpr(and_node->left);

        return Register(0);
    }

    Register Generator::lowerInt(const Node *node) {
        const auto int_str = m_ast.getSource(node);

        int64_t value = 0;
        // handle error
        std::from_chars(int_str.data(), int_str.data() + int_str.size(), value);

        Register reg = m_builder.create_load_imm_int(value);
        return reg;
    }
} // namespace Nyx::bytecode
