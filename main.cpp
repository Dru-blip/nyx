#include "bytecode/Compiler.h"
#include "parser/Ast.h"

int main() {
    Nyx::Ast ast = Nyx::Ast::parse("return 5");
    Nyx::Compiler compiler(ast);
    compiler.compile();
    return 0;
}
