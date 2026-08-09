#include "bytecode/Generator.h"
#include <charconv>
#include <corecrt_terminate.h>
#include <cstdint>
#include <string_view>
#include <utility>

#include "bytecode/Executable.h"
#include "bytecode/Operand.h"
#include "parser/Ast.h"


namespace Nyx::bytecode {
    Executable *Generator::build_executable() {
        return m_heap->alloc<Executable>(m_emitter.code());
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
            Operand result = lowerExpr(expr);

            if (result.isConstInt()) {
                m_emitter.emit_ret_imm(result.as.imm);
                return;
            }
        }

        m_emitter.emit_ret();
    }

    Operand Generator::lowerExpr(const Node *node) {
        switch (node->tag) {
            case NodeTag::Integer: {
                return lowerInt(node);
            }
            case NodeTag::Add: {
                return lowerAdd(node);
            }
            default: {
                abort();
            }
        }
    }

    Operand Generator::lowerAdd(const Node *node) {
        const Add *add = static_cast<const Add *>(node);
        Operand left = lowerExpr(add->left);
        Operand right = lowerExpr(add->right);

        if (left.isConstInt() && right.isConstInt()) {
            return {OperandType::ConstInt, {.imm = left.as.imm + right.as.imm}};
        }

        if (left.isConstInt() && right.isRegister()) {
            std::swap(left, right);
        }

        uint8_t reg = m_emitter.emit_add(left, right);
        return {OperandType::Register, {.reg = reg}};
    }

    Operand Generator::lowerInt(const Node *node) {
        const auto int_str = m_ast.getSource(node);

        int64_t value = 0;
        // handle error
        std::from_chars(int_str.data(), int_str.data() + int_str.size(), value);

        return {OperandType::ConstInt, {.imm = value}};
    }
} // namespace Nyx::bytecode
