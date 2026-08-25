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

    static uint16_t read_u16(uint8_t *&pc) {
        uint16_t val = static_cast<uint16_t>(pc[0]) | (static_cast<uint16_t>(pc[1]) << 8);
        pc += 2;
        return val;
    }

    static int64_t read_i64(uint8_t *&pc) {
        uint64_t val = 0;
        for (int i = 0; i < 8; ++i) {
            val |= (static_cast<uint64_t>(pc[i]) << (i * 8));
        }
        pc += 8;
        return static_cast<int64_t>(val);
    }

    // static uint8_t read_u8(uint8_t *&pc) { return *pc++; }

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
            int64_t imm = read_i64(pc);
            PUSH(Value(imm));
            DISPATCH();
        }

        HANDLE_INSTR(LoadConst) {
            uint16_t idx = read_u16(pc);
            PUSH(constants[idx]);
            DISPATCH();
        }
        HANDLE_INSTR(Neg) {
            *(stack - 1) = Handlers::handle_neg(*(stack - 1));
            DISPATCH();
        }
        HANDLE_INSTR(Not) {
            *(stack - 1) = Handlers::handle_not(*(stack - 1));
            DISPATCH();
        }
        HANDLE_INSTR(Add) {
            *(stack - 2) = Handlers::handle_add(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Sub) {
            *(stack - 2) = Handlers::handle_sub(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Mul) {
            *(stack - 2) = Handlers::handle_mul(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Div) {
            *(stack - 2) = Handlers::handle_div(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Lt) {
            *(stack - 2) = Handlers::handle_lt(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Lte) {
            *(stack - 2) = Handlers::handle_lte(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Gt) {
            *(stack - 2) = Handlers::handle_gt(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Gte) {
            *(stack - 2) = Handlers::handle_gte(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Eq) {
            *(stack - 2) = Handlers::handle_eq(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Neq) {
            *(stack - 2) = Handlers::handle_neq(*(stack - 2), *(stack - 1));
            stack--;
            DISPATCH();
        }
        HANDLE_INSTR(Jmp) {
            uint16_t offset = read_u16(pc);
            pc = frame->get_code() + offset;
            DISPATCH();
        }
        HANDLE_INSTR(JmpIfFalse) {
            uint16_t offset = read_u16(pc);
            if (!((POP()).is_truthy())) {
                pc = frame->get_code() + offset;
            }
            DISPATCH();
        }
        HANDLE_INSTR(JmpIfTrue) {
            uint16_t offset = read_u16(pc);
            if ((POP()).is_truthy()) {
                pc = frame->get_code() + offset;
            }
            DISPATCH();
        }
        HANDLE_INSTR(Ret) { return POP(); }
        HANDLE_INSTR(RetNil) { return Nil; }
    }
} // namespace Nyx
