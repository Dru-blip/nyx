#pragma once
#include <concepts>
#include <cstring>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "Token.h"
#include "mimalloc.h"


namespace Nyx {
    enum class NodeTag {
        Integer,

        Add,
        Sub,
        Mul,
        Div,


        Ret,
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

    struct Binary : Node {
        Binary(NodeTag tag, Span span, Node *left, Node *right) :
            Node(tag, span), left(left), right(right) {}
        Node *left, *right;
    };

    struct Add : Binary {
        Add(Span span, Node *left, Node *right) : Binary(NodeTag::Add, span, left, right) {}
    };

    struct Sub : Binary {
        Sub(Span span, Node *left, Node *right) : Binary(NodeTag::Sub, span, left, right) {}
    };

    struct Mul : Binary {
        Mul(Span span, Node *left, Node *right) : Binary(NodeTag::Mul, span, left, right) {}
    };

    struct Div : Binary {
        Div(Span span, Node *left, Node *right) : Binary(NodeTag::Div, span, left, right) {}
    };

    struct Return : public Node {
        Return(Span span, std::optional<Node *> value) : Node(NodeTag::Ret, span), value(value) {}
        std::optional<Node *> value;
    };

    class NodeArena {
    public:
        NodeArena();
        ~NodeArena();

        template<std::derived_from<Node> T, typename... Args>
        T *allocate(Args... args) {
            void *slot = mi_heap_malloc(m_heap, sizeof(T));
            T *node = new (slot) T(std::forward<Args>(args)...);
            return node;
        }

        std::span<Node *> nodes_span(std::vector<Node *> &nodes) {
            const size_t size = nodes.size() * sizeof(Node *);
            void *mem = mi_heap_malloc(m_heap, size);
            memcpy(mem, nodes.data(), size);

            return {static_cast<Node **>(mem), nodes.size()};
        }


    private:
        mi_heap_t *m_heap{nullptr};
    };

    class Ast {
    public:
        Ast(std::string_view source, NodeArena arena, std::span<Node *> roots) :
            arena(arena), roots(roots), source(source) {};

        static Ast parse(std::string_view source);

        uint32_t length() const;


    private:
        NodeArena arena;
        std::span<Node *> roots;
        std::string_view source;
    };
} // namespace Nyx
