#include "bytecode/Generator.h"
#include <charconv>
#include <corecrt_terminate.h>
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
            default: {
                abort();
            }
        }
    }

    uint8_t Generator::lowerNeg(const Node *node) {
        const Neg *neg = static_cast<const Neg *>(node);
        uint8_t child = lowerExpr(neg->child);

        uint8_t reg = m_emitter.neg(child);
        return reg;
    }

    uint8_t Generator::lowerNot(const Node *node) {
        const Not *notNode = static_cast<const Not *>(node);
        uint8_t child = lowerExpr(notNode->child);

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

    uint8_t Generator::lowerInt(const Node *node) {
        const auto int_str = m_ast.getSource(node);

        int64_t value = 0;
        // handle error
        std::from_chars(int_str.data(), int_str.data() + int_str.size(), value);

        uint8_t reg = m_emitter.load_imm_int(value);
        return reg;
    }
} // namespace Nyx::bytecode
