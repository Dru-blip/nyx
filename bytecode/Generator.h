#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "bytecode/Executable.h"
#include "bytecode/InstructionEmitter.h"
#include "bytecode/Operand.h"
#include "bytecode/RegisterAllocator.h"
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
        uint8_t lowerExpr(const Node *node);
        uint8_t lowerAdd(const Node *node);
        uint8_t lowerSub(const Node *node);
        uint8_t lowerMul(const Node *node);
        uint8_t lowerDiv(const Node *node);
        uint8_t lowerLt(const Node *node);
        uint8_t lowerNeg(const Node *node);
        uint8_t lowerNot(const Node *node);
        uint8_t lowerInt(const Node *node);

        Ast m_ast;
        InstructionEmitter m_emitter;
        std::shared_ptr<Heap> m_heap;
        std::vector<Value> m_constants;
    };
} // namespace Nyx::bytecode
