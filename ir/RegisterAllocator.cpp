#include "RegisterAllocator.h"
#include <cassert>
#include <cstdint>
#include "ir/Register.h"


namespace Nyx::ir {
    Register RegisterAllocator::allocate() {
        for (uint8_t i = 0; i < m_max_used; i++) {
            if (!m_used_registers[i]) {
                m_used_registers[i] = true;
                return Register(i);
            }
        }
        // TODO: should raise an error.
        assert(m_max_used < RegisterAllocator::MaxRegisters);
        m_used_registers[m_max_used] = true;
        return Register(m_max_used++);
    }

    void RegisterAllocator::free(Register reg) { m_used_registers[reg.slot()] = false; }
} // namespace Nyx::ir
