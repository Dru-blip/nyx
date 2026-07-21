#pragma once


#include <cstdint>
#include <vector>
#include "parser/Ast.h"

namespace Nyx {
    class Compiler {
    public:
        Compiler(Ast &ast) : m_ast(std::move(ast)) {};
        void compile();

        size_t code_size() const { return m_code.size(); }


    private:
        void lowerRoot(const Node &node);
        void lowerRet(const Node &node);
        void lowerExpr(const Node &node);
        void lowerInt(const Node &node);
        void emit(uint8_t byte);

        Ast m_ast;
        std::vector<uint8_t> m_code;
    };
} // namespace Nyx
