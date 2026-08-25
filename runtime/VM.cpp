#include "runtime/VM.h"
#include <cstdint>
#include "runtime/Builtins.h"
#include "runtime/NativeFunction.h"
#include "runtime/Value.h"
#include "runtime/handlers/binary.h"


namespace Nyx {
    VM::VM() {
        void *ptr = mi_malloc(sizeof(Heap));
        m_heap = new (ptr) Heap();

        void *string_pool_ptr = mi_malloc(sizeof(StringPool));
        m_string_pool = new (string_pool_ptr) StringPool(*this);

        m_global_object = m_heap->alloc<GlobalObject>();

        Builtins::register_builtins(*this);
    }

    VM::~VM() {
        m_heap->~Heap();
        mi_free(m_heap);

        m_string_pool->~StringPool();
        mi_free(m_string_pool);
    }

    uint32_t VM::register_builtin_function(std::string_view &name, NativeFunctionPtr func) {
        String *identifier = m_string_pool->add_string(name);
        return m_global_object->put_native_function(*this, identifier, func);
    }

    Value VM::run_executable(bytecode::Executable *executable) {
        Frame *frame = m_fiber.push_frame(executable);

        uint8_t *pc = frame->get_code();
        Value *stack = frame->stack();
        // Value *locals = frame->locals();
        Value *constants = frame->constants();


        static const void *labels[] = {
                &&HandleLoadImmInt, &&HandleLoadConst, &&HandleNeg, &&HandleNot,    &&HandleAdd,
                &&HandleSub,        &&HandleMul,       &&HandleDiv, &&HandleLt,     &&HandleLte,
                &&HandleGt,         &&HandleGte,       &&HandleEq,  &&HandleNeq,    &&HandleJmp,
                &&HandleJmpIfFalse, &&HandleJmpIfTrue, &&HandleRet, &&HandleRetNil,
        };

#define PUSH(v) *stack++ = v
#define POP() *(--stack)
#define DISPATCH() goto *labels[*pc++]

        DISPATCH();

#define HANDLE_INSTR(name) Handle##name:

        HANDLE_INSTR(LoadImmInt) {
            bytecode::LoadImmInt instr = frame->read_at<bytecode::LoadImmInt>(pc);
            PUSH(Value(instr.imm));
            DISPATCH();
        }

        HANDLE_INSTR(LoadConst) {
            bytecode::LoadConst instr = frame->read_at<bytecode::LoadConst>(pc);
            PUSH(constants[instr.idx]);
            DISPATCH();
        }
        HANDLE_INSTR(Neg) {
            frame->read_at<bytecode::Neg>(pc);
            *(stack - 1) = Handlers::handle_neg(*(stack - 1));
            DISPATCH();
        }
        HANDLE_INSTR(Not) {
            frame->read_at<bytecode::Not>(pc);
            *(stack - 1) = Handlers::handle_not(*(stack - 1));
            DISPATCH();
        }
        HANDLE_INSTR(Add) {
            frame->read_at<bytecode::Add>(pc);
            *(stack - 2) = Handlers::handle_add(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Sub) {
            frame->read_at<bytecode::Sub>(pc);
            *(stack - 2) = Handlers::handle_sub(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Mul) {
            frame->read_at<bytecode::Mul>(pc);
            *(stack - 2) = Handlers::handle_mul(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Div) {
            frame->read_at<bytecode::Div>(pc);
            *(stack - 2) = Handlers::handle_div(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Lt) {
            frame->read_at<bytecode::Lt>(pc);
            *(stack - 2) = Handlers::handle_lt(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Lte) {
            frame->read_at<bytecode::Lte>(pc);
            *(stack - 2) = Handlers::handle_lte(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Gt) {
            frame->read_at<bytecode::Gt>(pc);
            *(stack - 2) = Handlers::handle_gt(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Gte) {
            frame->read_at<bytecode::Gte>(pc);
            *(stack - 2) = Handlers::handle_gte(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Eq) {
            frame->read_at<bytecode::Eq>(pc);
            *(stack - 2) = Handlers::handle_eq(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Neq) {
            frame->read_at<bytecode::Neq>(pc);
            *(stack - 2) = Handlers::handle_neq(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Jmp) {
            bytecode::Jmp instr = frame->read_at<bytecode::Jmp>(pc);
            pc = frame->get_code() + instr.offset;
            DISPATCH();
        }
        HANDLE_INSTR(JmpIfFalse) {
            bytecode::JmpIfFalse instr = frame->read_at<bytecode::JmpIfFalse>(pc);
            if (!((POP()).is_truthy())) {
                pc = frame->get_code() + instr.offset;
            }
            DISPATCH();
        }
        HANDLE_INSTR(JmpIfTrue) {
            bytecode::JmpIfTrue instr = frame->read_at<bytecode::JmpIfTrue>(pc);
            if ((POP()).is_truthy()) {
                pc = frame->get_code() + instr.offset;
            }
            DISPATCH();
        }
        HANDLE_INSTR(Ret) {
            frame->read_at<bytecode::Ret>(pc);
            return POP();
        }
        HANDLE_INSTR(RetNil) { return Nil; }
    }
} // namespace Nyx
