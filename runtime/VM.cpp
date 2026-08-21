#include "runtime/VM.h"
#include <cstdint>
#include "runtime/Value.h"
#include "runtime/handlers/binary.h"


namespace Nyx {
    VM::VM() {
        void *ptr = mi_malloc(sizeof(Heap));
        m_heap = new (ptr) Heap();

        void *string_pool_ptr = mi_malloc(sizeof(StringPool));
        m_string_pool = new (string_pool_ptr) StringPool(m_heap);
    }

    VM::~VM() {
        m_heap->~Heap();
        mi_free(m_heap);

        m_string_pool->~StringPool();
        mi_free(m_string_pool);
    }

    Value VM::run_executable(bytecode::Executable *executable) {
        Frame *frame = m_fiber.push_frame(executable);

        uint8_t *pc = frame->get_code();
        Value *registers = frame->registers();
        Value *constants = frame->constants();

        static const void *labels[] = {
                &&HandleLoadImmInt, &&HandleLoadConst, &&HandleMove, &&HandleNeg,
                &&HandleNot,        &&HandleAdd,       &&HandleSub,  &&HandleMul,
                &&HandleDiv,        &&HandleLt,        &&HandleLte,  &&HandleGt,
                &&HandleGte,        &&HandleEq,        &&HandleNeq,  &&HandleJmp,
                &&HandleJmpIfFalse, &&HandleJmpIfTrue, &&HandleRet,  &&HandleRetNil,
        };

#define DISPATCH() goto *labels[*pc++]

        DISPATCH();

#define HANDLE_INSTR(name) Handle##name:

        HANDLE_INSTR(LoadImmInt) {
            bytecode::LoadImmInt instr = frame->read_at<bytecode::LoadImmInt>(pc);
            registers[instr.reg] = Value(instr.imm);
            DISPATCH();
        }

        HANDLE_INSTR(LoadConst) {
            bytecode::LoadConst instr = frame->read_at<bytecode::LoadConst>(pc);
            registers[instr.reg] = constants[instr.idx];
            DISPATCH();
        }

        HANDLE_INSTR(Move) {
            bytecode::Move instr = frame->read_at<bytecode::Move>(pc);
            registers[instr.dst] = registers[instr.src];
            DISPATCH();
        }

        HANDLE_INSTR(Neg) {
            bytecode::Neg instr = frame->read_at<bytecode::Neg>(pc);
            registers[instr.result] = Handlers::handle_neg(registers[instr.arg]);
            DISPATCH();
        }


        HANDLE_INSTR(Not) {
            bytecode::Not instr = frame->read_at<bytecode::Not>(pc);
            registers[instr.result] = Handlers::handle_not(registers[instr.arg]);
            DISPATCH();
        }
        HANDLE_INSTR(Add) {
            bytecode::Add instr = frame->read_at<bytecode::Add>(pc);
            registers[instr.reg] = Handlers::handle_add(registers[instr.lhs], registers[instr.rhs]);
            DISPATCH();
        }
        HANDLE_INSTR(Sub) {
            bytecode::Sub instr = frame->read_at<bytecode::Sub>(pc);
            registers[instr.reg] = Handlers::handle_sub(registers[instr.lhs], registers[instr.rhs]);
            DISPATCH();
        }
        HANDLE_INSTR(Mul) {
            bytecode::Mul instr = frame->read_at<bytecode::Mul>(pc);
            registers[instr.reg] = Handlers::handle_mul(registers[instr.lhs], registers[instr.rhs]);
            DISPATCH();
        }
        HANDLE_INSTR(Div) {
            bytecode::Div instr = frame->read_at<bytecode::Div>(pc);
            registers[instr.reg] = Handlers::handle_div(registers[instr.lhs], registers[instr.rhs]);
            DISPATCH();
        }
        HANDLE_INSTR(Lt) {
            bytecode::Lt instr = frame->read_at<bytecode::Lt>(pc);
            registers[instr.reg] = Handlers::handle_lt(registers[instr.lhs], registers[instr.rhs]);
            DISPATCH();
        }
        HANDLE_INSTR(Lte) {
            bytecode::Lte instr = frame->read_at<bytecode::Lte>(pc);
            registers[instr.reg] = Handlers::handle_lte(registers[instr.lhs], registers[instr.rhs]);
            DISPATCH();
        }
        HANDLE_INSTR(Gt) {
            bytecode::Gt instr = frame->read_at<bytecode::Gt>(pc);
            registers[instr.reg] = Handlers::handle_gt(registers[instr.lhs], registers[instr.rhs]);
            DISPATCH();
        }
        HANDLE_INSTR(Gte) {
            bytecode::Gte instr = frame->read_at<bytecode::Gte>(pc);
            registers[instr.reg] = Handlers::handle_gte(registers[instr.lhs], registers[instr.rhs]);
            DISPATCH();
        }
        HANDLE_INSTR(Eq) {
            bytecode::Eq instr = frame->read_at<bytecode::Eq>(pc);
            registers[instr.reg] = Handlers::handle_eq(registers[instr.lhs], registers[instr.rhs]);
            DISPATCH();
        }
        HANDLE_INSTR(Neq) {
            bytecode::Neq instr = frame->read_at<bytecode::Neq>(pc);
            registers[instr.reg] = Handlers::handle_neq(registers[instr.lhs], registers[instr.rhs]);
            DISPATCH();
        }
        HANDLE_INSTR(Jmp) {
            bytecode::Jmp instr = frame->read_at<bytecode::Jmp>(pc);
            pc = frame->get_code() + instr.offset;
            DISPATCH();
        }
        HANDLE_INSTR(JmpIfFalse) {
            bytecode::JmpIfFalse instr = frame->read_at<bytecode::JmpIfFalse>(pc);

            if (!registers[instr.arg].is_truthy()) {
                pc = frame->get_code() + instr.offset;
            }

            DISPATCH();
        }
        HANDLE_INSTR(JmpIfTrue) {
            bytecode::JmpIfTrue instr = frame->read_at<bytecode::JmpIfTrue>(pc);

            if (registers[instr.arg].is_truthy()) {
                pc = frame->get_code() + instr.offset;
            }

            DISPATCH();
        }
        HANDLE_INSTR(Ret) {
            bytecode::Ret instr = frame->read_at<bytecode::Ret>(pc);
            return registers[instr.reg];
        }
        HANDLE_INSTR(RetNil) { return Nil; }
    }
} // namespace Nyx
