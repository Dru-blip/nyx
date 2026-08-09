#include "RegisterAllocator.h"
#include <cassert>


namespace Nyx::bytecode {
    uint8_t RegisterAllocator::allocate() { return m_next_register++; }

    void RegisterAllocator::free(uint8_t reg) {
        assert(m_next_register - 1 == reg);
        m_next_register--;
    }
} // namespace Nyx::bytecode
