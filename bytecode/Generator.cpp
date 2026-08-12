#include "bytecode/Generator.h"
#include <charconv>
#include <cstdint>
#include <string_view>

#include "bytecode/Executable.h"
#include "parser/Ast.h"


namespace Nyx::bytecode {
    Executable *Generator::build_executable() {
        return m_heap->alloc<Executable>(m_emitter.code(), m_constants);
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
            uint8_t result = lowerExpr(expr);
            m_emitter.ret(result);
            return;
        }
    }

    uint8_t Generator::lowerExpr(const Node *node) {
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
            default: {
                abort();
            }
        }
    }

    uint8_t Generator::lowerGrouped(const Node *node) {
        const Unary *grouped = static_cast<const Unary *>(node);
        uint8_t child = lowerExpr(grouped->arg);
        return child;
    }

    uint8_t Generator::lowerNeg(const Node *node) {
        const Unary *neg = static_cast<const Unary *>(node);
        uint8_t child = lowerExpr(neg->arg);

        uint8_t reg = m_emitter.neg(child);
        return reg;
    }

    uint8_t Generator::lowerNot(const Node *node) {
        const Unary *notNode = static_cast<const Unary *>(node);
        uint8_t child = lowerExpr(notNode->arg);

        uint8_t reg = m_emitter.not_(child);
        return reg;
    }

    uint8_t Generator::lowerAdd(const Node *node) {
        const Binary *add = static_cast<const Binary *>(node);
        uint8_t left = lowerExpr(add->left);
        uint8_t right = lowerExpr(add->right);

        uint8_t reg = m_emitter.add(left, right);
        return reg;
    }

    uint8_t Generator::lowerSub(const Node *node) {
        const Binary *sub = static_cast<const Binary *>(node);
        uint8_t left = lowerExpr(sub->left);
        uint8_t right = lowerExpr(sub->right);

        uint8_t reg = m_emitter.sub(left, right);
        return reg;
    }


    uint8_t Generator::lowerMul(const Node *node) {
        const Binary *mul = static_cast<const Binary *>(node);
        uint8_t left = lowerExpr(mul->left);
        uint8_t right = lowerExpr(mul->right);

        uint8_t reg = m_emitter.mul(left, right);
        return reg;
    }

    uint8_t Generator::lowerDiv(const Node *node) {
        const Binary *div = static_cast<const Binary *>(node);
        uint8_t left = lowerExpr(div->left);
        uint8_t right = lowerExpr(div->right);

        uint8_t reg = m_emitter.div(left, right);
        return reg;
    }

    uint8_t Generator::lowerLt(const Node *node) {
        const Binary *lt = static_cast<const Binary *>(node);
        uint8_t left = lowerExpr(lt->left);
        uint8_t right = lowerExpr(lt->right);

        uint8_t reg = m_emitter.lt(left, right);
        return reg;
    }


    uint8_t Generator::lowerLte(const Node *node) {
        const Binary *lte = static_cast<const Binary *>(node);
        uint8_t left = lowerExpr(lte->left);
        uint8_t right = lowerExpr(lte->right);

        uint8_t reg = m_emitter.lte(left, right);
        return reg;
    }

    uint8_t Generator::lowerGt(const Node *node) {
        const Binary *gt = static_cast<const Binary *>(node);
        uint8_t left = lowerExpr(gt->left);
        uint8_t right = lowerExpr(gt->right);

        uint8_t reg = m_emitter.gt(left, right);
        return reg;
    }

    uint8_t Generator::lowerGte(const Node *node) {
        const Binary *gte = static_cast<const Binary *>(node);
        uint8_t left = lowerExpr(gte->left);
        uint8_t right = lowerExpr(gte->right);

        uint8_t reg = m_emitter.gte(left, right);
        return reg;
    }

    uint8_t Generator::lowerEq(const Node *node) {
        const Binary *eq = static_cast<const Binary *>(node);
        uint8_t left = lowerExpr(eq->left);
        uint8_t right = lowerExpr(eq->right);

        uint8_t reg = m_emitter.eq(left, right);
        return reg;
    }

    uint8_t Generator::lowerNeq(const Node *node) {
        const Binary *neq = static_cast<const Binary *>(node);
        uint8_t left = lowerExpr(neq->left);
        uint8_t right = lowerExpr(neq->right);

        uint8_t reg = m_emitter.neq(left, right);
        return reg;
    }


    uint8_t Generator::lowerInt(const Node *node) {
        const auto int_str = m_ast.getSource(node);

        int64_t value = 0;
        // handle error
        std::from_chars(int_str.data(), int_str.data() + int_str.size(), value);

        uint8_t reg = m_emitter.load_imm_int(value);
        return reg;
    }
} // namespace Nyx::bytecode
