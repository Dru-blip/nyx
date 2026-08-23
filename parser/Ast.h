#pragma once

#include <concepts>
#include <cstring>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

#include "Token.h"
#include "mimalloc.h"


namespace Nyx {
    enum class NodeTag {
        Integer,
        String,
        Identifier,

        Neg,
        Not,

        Add,
        Sub,
        Mul,
        Div,

        Lt,
        Gt,
        Gte,
        Lte,
        Eq,
        Neq,

        And,
        Or,

        Grouped,
        Call,
        Assignment,

        ExprStmt,
        Ret,
        BlockStmt,
        If,
        Loop,
        Break,
        Continue,
        For,
        While,

        VarDecl,
    };

    struct Node {
        NodeTag tag;
        Span span{};

        Node(NodeTag tag, Span span) : tag(tag), span(span) {}
    };

    struct IntLiteral : Node {
        IntLiteral(Span span) : Node(NodeTag::Integer, span) {}
    };

    struct StringLiteral : Node {
        StringLiteral(Span span) : Node(NodeTag::String, span) {}
    };

    struct Identifier : Node {
        Identifier(Span span) : Node(NodeTag::Identifier, span) {}
    };

    struct Unary : public Node {
        Unary(NodeTag tag, Span span, Node *arg) : Node(tag, span), arg(arg) {}
        Node *arg;
    };

    struct Binary : Node {
        Binary(NodeTag tag, Span span, Node *left, Node *right) :
            Node(tag, span), left(left), right(right) {}
        Node *left, *right;
    };

    struct CallExpr : Node {
        CallExpr(Span span, Node *callee, std::span<Node *> args) :
            Node(NodeTag::Call, span), callee(callee), args(args) {}
        Node *callee;
        std::span<Node *> args;
    };

    struct ExprStmt : Node {
        ExprStmt(Span span, Node *expr) : Node(NodeTag::ExprStmt, span), expr(expr) {}
        Node *expr;
    };

    struct Return : Node {
        Return(Span span, std::optional<Node *> value) : Node(NodeTag::Ret, span), value(value) {}
        std::optional<Node *> value;
    };

    struct BlockStmt : Node {
        BlockStmt(Span span, std::span<Node *> stmts) :
            Node(NodeTag::BlockStmt, span), stmts(stmts) {}
        std::span<Node *> stmts;
    };

    struct If : Node {
        If(Span span, Node *test, Node *consequent, Node *alternate) :
            Node(NodeTag::If, span), test(test), consequent(consequent), alternate(alternate) {}
        Node *test, *consequent, *alternate;
    };

    struct Break : Node {
        Break(Span span) : Node(NodeTag::Break, span) {}
    };

    struct Continue : Node {
        Continue(Span span) : Node(NodeTag::Continue, span) {}
    };

    struct Loop : Node {
        Loop(Span span, Node *body) : Node(NodeTag::Loop, span), body(body) {}
        Node *body;
    };

    struct WhileLoop : Node {
        WhileLoop(Span span, Node *test, Node *body) :
            Node(NodeTag::While, span), test(test), body(body) {}
        Node *test, *body;
    };

    struct VarDecl : Node {
        VarDecl(Span span, Span name, Node *value) :
            Node(NodeTag::VarDecl, span), name(name), initializer(value) {}
        Span name;
        Node *initializer;
    };

    class NodeArena {
    public:
        NodeArena();
        ~NodeArena();

        NodeArena(NodeArena &&other) noexcept {
            m_heap = other.m_heap;
            other.m_heap = nullptr;
        }

        template<std::derived_from<Node> T, typename... Args>
        T *allocate(Args &&...args) {
            void *slot = mi_heap_malloc(m_heap, sizeof(T));
            T *node = new (slot) T(std::forward<Args>(args)...);
            return node;
        }

        std::span<Node *> nodes_span(std::vector<Node *> &nodes) {
            const size_t size = nodes.size() * sizeof(Node *);
            void *mem = mi_heap_malloc(m_heap, size);
            std::memcpy(mem, nodes.data(), size);
            return {static_cast<Node **>(mem), nodes.size()};
        }

    private:
        mi_heap_t *m_heap{nullptr};
    };

    class Ast {
    public:
        Ast(std::string_view source, NodeArena &arena, std::span<Node *> roots) :
            m_arena(std::move(arena)), m_roots(roots), m_source(source) {};

        static Ast parse(std::string_view source);

        uint32_t length() const;
        std::span<Node *> roots() const { return m_roots; }

        std::string_view getSource(const Node *node) const {
            return m_source.substr(node->span.start, node->span.end - node->span.start);
        }

        std::string_view getSource(const Span &span) const {
            return m_source.substr(span.start, span.end - span.start);
        }

    private:
        NodeArena m_arena;
        std::span<Node *> m_roots;
        std::string_view m_source;
    };
} // namespace Nyx
