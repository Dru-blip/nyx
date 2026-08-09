#include "RegisterAllocator.h"
#include <cassert>


namespace Nyx::bytecode {
    uint8_t RegisterAllocator::allocate() {
        if (m_next_register > m_max_used) {
            m_max_used = m_next_register;
        }
        return m_next_register++;
    }

    void RegisterAllocator::free(uint8_t reg) {
        assert(m_next_register - 1 == reg);
        m_next_register--;
    }
} // namespace Nyx::bytecode
