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

        ExprStmt,
        Ret,
        BlockStmt,

        VarDecl,
    };

    struct Node {
        NodeTag tag;
        Span span{};

        Node(NodeTag tag, Span span) : tag(tag), span(span) {}
        virtual ~Node() = default;
    };

    struct IntLiteral : Node {
        IntLiteral(Span span) : Node(NodeTag::Integer, span) {}
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

    private:
        NodeArena m_arena;
        std::span<Node *> m_roots;
        std::string_view m_source;
    };
} // namespace Nyx
