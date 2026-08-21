#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "bytecode/Executable.h"
#include "bytecode/Scope.h"
#include "heap/Heap.h"
#include "ir/Builder.h"
#include "ir/Register.h"
#include "parser/Ast.h"
#include "runtime/String.h"

namespace Nyx::bytecode {
    using BasicBlock = ir::BasicBlock;


    struct LoopInfo {
        BasicBlock *header;
        BasicBlock *end;
    };

    class Generator {
    public:
        Generator(Ast &ast, std::shared_ptr<Heap> heap) : m_ast(std::move(ast)), m_heap(heap) {};
        Executable *compile();

    private:
        Executable *build_executable();
        std::size_t make_block();

        void lowerRoot(const Node *node);
        void lowerVarDecl(const Node *node);
        void lowerWhileLoop(const Node *node);
        void lowerLoop(const Node *node);
        void lowerBreak(const Node *node);
        void lowerContinue(const Node *node);
        void lowerIf(const Node *node);
        void lowerBlockStmt(const Node *node);
        void lowerRet(const Node *node);
        void lowerExprStmt(const Node *node);

        ir::Register lowerExpr(const Node *node);
        ir::Register lowerGrouped(const Node *node);
        ir::Register lowerAdd(const Node *node);
        ir::Register lowerSub(const Node *node);
        ir::Register lowerMul(const Node *node);
        ir::Register lowerDiv(const Node *node);
        ir::Register lowerLt(const Node *node);
        ir::Register lowerLte(const Node *node);
        ir::Register lowerGt(const Node *node);
        ir::Register lowerGte(const Node *node);
        ir::Register lowerEq(const Node *node);
        ir::Register lowerNeq(const Node *node);

        ir::Register lowerAnd(const Node *node);
        ir::Register lowerOr(const Node *node);

        ir::Register lowerAssignment(const Node *node);

        ir::Register lowerNeg(const Node *node);
        ir::Register lowerNot(const Node *node);
        ir::Register lowerIdentifier(const Node *node);
        ir::Register lowerString(const Node *node);
        ir::Register lowerInt(const Node *node);

        uint16_t add_string_constant(const char *data, std::size_t size);

        Ast m_ast;
        ir::Builder m_builder;
        std::shared_ptr<Heap> m_heap;
        std::vector<Value> m_constants;
        std::vector<LoopInfo> m_loop_stack;
        Scope m_scope;
    };
} // namespace Nyx::bytecode
