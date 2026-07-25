#pragma once

#include <cstdint>
#include <vector>
#include "bytecode/InstructionEmitter.h"
#include "bytecode/Operand.h"
#include "parser/Ast.h"

namespace Nyx::bytecode {
    class Generator {
    public:
        Generator(Ast &ast) : m_ast(std::move(ast)) {};
        void compile();

    private:
        void lowerRoot(const Node &node);
        void lowerRet(const Node &node);
        Operand lowerExpr(const Node &node);
        Operand lowerInt(const Node &node);

        Ast m_ast;
        InstructionEmitter m_emitter;
    };
} // namespace Nyx::bytecode
