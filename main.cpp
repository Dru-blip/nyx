#include <filesystem>
#include <fstream>
#include <memory>
#include <print>
#include <sstream>
#include "bytecode/Executable.h"
#include "bytecode/Generator.h"
#include "heap/Heap.h"
#include "parser/Ast.h"
#include "runtime/Fiber.h"
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

    Nyx::Ast ast = Nyx::Ast::parse(source);
    std::shared_ptr<Nyx::Heap> heap = std::make_shared<Nyx::Heap>();

    Nyx::bytecode::Generator generator(ast, heap);
    Nyx::bytecode::Executable *executable = generator.compile();

    executable->print_code();

    Nyx::Fiber fiber;
    Nyx::Value result = fiber.run(executable);

    std::printf("%p\n", static_cast<void *>(result.as_obj()));
    return 0;
}
