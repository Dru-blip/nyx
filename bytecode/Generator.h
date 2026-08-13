#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "bytecode/Executable.h"
#include "heap/Heap.h"
#include "ir/Builder.h"
#include "ir/Register.h"
#include "parser/Ast.h"

namespace Nyx::bytecode {
    using Register = ir::Register;
    class Generator {
    public:
        Generator(Ast &ast, std::shared_ptr<Heap> heap) : m_ast(std::move(ast)), m_heap(heap) {};
        Executable *compile();

    private:
        Executable *build_executable();
        std::size_t make_block();

        void lowerRoot(const Node *node);
        void lowerRet(const Node *node);
        Register lowerExpr(const Node *node);
        Register lowerGrouped(const Node *node);
        Register lowerAdd(const Node *node);
        Register lowerSub(const Node *node);
        Register lowerMul(const Node *node);
        Register lowerDiv(const Node *node);
        Register lowerLt(const Node *node);
        Register lowerLte(const Node *node);
        Register lowerGt(const Node *node);
        Register lowerGte(const Node *node);
        Register lowerEq(const Node *node);
        Register lowerNeq(const Node *node);

        Register lowerAnd(const Node *node);

        Register lowerNeg(const Node *node);
        Register lowerNot(const Node *node);
        Register lowerInt(const Node *node);

        Ast m_ast;
        ir::Builder m_builder;
        std::shared_ptr<Heap> m_heap;
        std::vector<Value> m_constants;
    };
} // namespace Nyx::bytecode
