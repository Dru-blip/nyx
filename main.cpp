#include <filesystem>
#include <fstream>
#include <memory>
#include <print>
#include <sstream>
#include "bytecode/Executable.h"
#include "bytecode/Generator.h"
#include "heap/Heap.h"
#include "parser/Ast.h"
#include "runtime/VM.h"
#include "runtime/Value.h"

std::string read_file(const std::filesystem::path &path) {
    std::ifstream file(path);

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::println("Usage: {} <file>", argv[0]);
        return 1;
    }

    std::string source = read_file(std::filesystem::path(argv[1]));

    Nyx::VM vm;
    Nyx::Ast ast = Nyx::Ast::parse(source);
    Nyx::bytecode::Generator generator(ast, vm.heap());
    Nyx::bytecode::Executable *executable = generator.compile();

    executable->print_code();

    Nyx::Value result = vm.run_executable(executable);
    std::println("{}", result.as_int());

    return 0;
}
