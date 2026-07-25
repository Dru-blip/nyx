#include "bytecode/Generator.h"
#include <charconv>
#include <cstdint>
#include <cstdio>
#include <string_view>

#include "bytecode/Operand.h"
#include "parser/Ast.h"


namespace Nyx::bytecode {
    void Generator::compile() {
        const Node &rootNode = m_ast.node(0);
        const NodeRange roots = std::get<NodeRange>(rootNode.data);

        for (NodeIndex extra_index = roots.start; extra_index < roots.end; ++extra_index) {
            const NodeIndex index = m_ast.extra(extra_index);
            lowerRoot(m_ast.node(index));
        }
    }

    void Generator::lowerRoot(const Node &node) {
        switch (node.tag) {
            case NodeTag::Ret: {
                lowerRet(node);
            }
            default: {
            }
        }
    }

    void Generator::lowerRet(const Node &node) {
        const auto index = node.index();
        if (index.has_value()) {
            const Node &expr = m_ast.node(index.value());
            Operand result = lowerExpr(expr);

            if (result.isConstInt()) {
                m_emitter.emit_ret_imm(result.as.imm);
                return;
            }
        }

        m_emitter.emit_ret();
    }

    Operand Generator::lowerExpr(const Node &node) {
        switch (node.tag) {
            case NodeTag::Integer: {
                return lowerInt(node);
            }
            default: {
            }
        }
    }


    Operand Generator::lowerInt(const Node &node) {
        const auto int_str = m_ast.getSource(node);

        int64_t value = 0;
        // handle error
        std::from_chars(int_str.data(), int_str.data() + int_str.size(), value);

        return {OperandType::ConstInt, {.imm = value}};
    }
} // namespace Nyx::bytecode
