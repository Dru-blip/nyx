#pragma once

#include <cstdint>
#include <vector>
#include "bytecode/Executable.h"
#include "bytecode/Scope.h"
#include "ir/Builder.h"
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
        Generator(Ast &ast, VM &vm) : m_ast(std::move(ast)), m_vm(vm) {};
        Executable *compile();

    private:
        Executable *build_executable();
        std::size_t make_block();

        void lowerRoot(const Node *node);
        void lowerFnDecl(const Node *node);
        void lowerVarDecl(const Node *node);
        void lowerWhileLoop(const Node *node);
        void lowerLoop(const Node *node);
        void lowerBreak(const Node *node);
        void lowerContinue(const Node *node);
        void lowerIf(const Node *node);
        void lowerBlockStmt(const Node *node);
        void lowerRet(const Node *node);
        void lowerExprStmt(const Node *node);

        void lowerExpr(const Node *node);
        void lowerCall(const Node *node);
        void lowerGrouped(const Node *node);
        void lowerAdd(const Node *node);
        void lowerSub(const Node *node);
        void lowerMul(const Node *node);
        void lowerDiv(const Node *node);
        void lowerLt(const Node *node);
        void lowerLte(const Node *node);
        void lowerGt(const Node *node);
        void lowerGte(const Node *node);
        void lowerEq(const Node *node);
        void lowerNeq(const Node *node);

        void lowerAnd(const Node *node);
        void lowerOr(const Node *node);

        void lowerAssignment(const Node *node);

        void lowerNeg(const Node *node);
        void lowerNot(const Node *node);
        void lowerIdentifier(const Node *node);
        void lowerString(const Node *node);
        void lowerInt(const Node *node);

        uint16_t add_string_constant(const char *data, std::size_t size);
        uint16_t add_int_constant(int64_t value);

        int resolve_global(const std::string_view &name);
        uint16_t intern_string_and_constant(std::string_view data);

        Ast m_ast;
        ir::Builder m_builder;
        VM &m_vm;
        std::vector<Value> m_constants;
        std::vector<LoopInfo> m_loop_stack;
        Scope m_scope;
    };
} // namespace Nyx::bytecode
