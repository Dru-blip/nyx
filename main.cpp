#include "bytecode/Compiler.h"
#include "parser/Ast.h"

int main() {
    Nyx::Ast ast = Nyx::Ast::parse("return 5");
    std::printf("total nodes : %d\n", ast.length());

    Nyx::Compiler compiler(ast);
    compiler.compile();

    std::printf("code size %lld\n", compiler.code_size());

    return 0;
}
