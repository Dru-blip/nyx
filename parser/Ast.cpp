#include "Ast.h"

#include "Parser.h"

namespace Nyx {
    Ast Ast::parse(const std::string_view source) {
        Parser parser(source);
        parser.parse();
        return parser.into_ast();
    }

    uint32_t Ast::length() const  {
        return m_nodes.size();
    }
}
