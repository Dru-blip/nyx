#include "parser/Ast.h"

int main() {
    const Nyx::Ast ast = Nyx::Ast::parse("return 5");
    std::printf("total nodes : %d", ast.length());

    return 0;
}
