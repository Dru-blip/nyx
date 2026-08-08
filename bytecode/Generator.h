#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "bytecode/Executable.h"
#include "bytecode/InstructionEmitter.h"
#include "bytecode/Operand.h"
#include "heap/Heap.h"
#include "parser/Ast.h"

namespace Nyx::bytecode {
    class Generator {
    public:
        Generator(Ast &ast, std::shared_ptr<Heap> heap) : m_ast(std::move(ast)), m_heap(heap) {};
        Executable *compile();
        Executable *build_executable();

    private:
        void lowerRoot(const Node *node);
        void lowerRet(const Node *node);
        Operand lowerExpr(const Node *node);
        Operand lowerAdd(const Node *node);
        Operand lowerInt(const Node *node);

        Ast m_ast;
        InstructionEmitter m_emitter;
        std::shared_ptr<Heap> m_heap;
    };
} // namespace Nyx::bytecode
