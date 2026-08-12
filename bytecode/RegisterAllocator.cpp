#include "RegisterAllocator.h"
#include <cassert>
#include <cstdint>


namespace Nyx::bytecode {
    uint8_t RegisterAllocator::allocate() {
        for (uint8_t i = 0; i < m_max_used; i++) {
            if (!m_used_registers[i]) {
                m_used_registers[i] = true;
                return i;
            }
        }
        // TODO: should raise an error.
        assert(m_max_used < RegisterAllocator::MaxRegisters);
        m_used_registers[m_max_used] = true;
        return m_max_used++;
    }

    void RegisterAllocator::free(uint8_t reg) { m_used_registers[reg] = false; }
} // namespace Nyx::bytecode
