#include "bytecode/Compiler.h"
#include <charconv>
#include <cstdint>
#include <string_view>
#include "bytecode/Opcode.h"
#include "parser/Ast.h"


namespace Nyx {
    void Compiler::compile() {
        const Node &rootNode = m_ast.node(0);
        const NodeRange roots = std::get<NodeRange>(rootNode.data);

        for (NodeIndex index = roots.start; index < roots.end; ++index) {
            lowerRoot(m_ast.node(index));
        }
    }

    void Compiler::lowerRoot(const Node &node) {
        switch (node.tag) {
            case NodeTag::Ret: {
                lowerRet(node);
            }
            default: {
            }
        }
    }

    void Compiler::lowerRet(const Node &node) {
        const auto index = node.index();
        if (index.has_value()) {
            const Node &expr = m_ast.node(index.value());
            lowerExpr(expr);
        }
        emit(static_cast<uint8_t>(Opcode::Ret));
    }

    void Compiler::lowerExpr(const Node &node) {
        switch (node.tag) {
            case NodeTag::Integer: {
                lowerInt(node);
                break;
            }
            default: {
            }
        }
    }


    void Compiler::lowerInt(const Node &node) {
        const auto int_str = m_ast.getSource(node);

        int64_t value = 0;
        // handle error
        std::from_chars(int_str.data(), int_str.data() + int_str.size(), value);

        emit(static_cast<uint8_t>(Opcode::LdaImmInt));

        for (uint8_t i = 0; i < 8; i++) {
            emit(static_cast<uint8_t>(value >> (8 * i) & 0xff));
        }
    }


    void Compiler::emit(uint8_t byte) { m_code.push_back(byte); }
} // namespace Nyx
