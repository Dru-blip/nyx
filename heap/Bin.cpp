#include "heap/Bin.h"
#include <cassert>
#include "heap/Block.h"
#include "heap/Cell.h"


namespace Nyx {
    void *Bin::allocate(size_t size) {
        try_acquire_block(size);
        void *cell = m_active->get_free_cell();
        return cell;
    }

    void Bin::try_acquire_block(size_t size) {
        if (m_active && m_active->has_free_cells()) {
            return;
        }

        Block *block = m_block_allocator.acquire_block();
        assert(block != nullptr);
        block->format(size);
        m_active = block;
        m_used.push_back(block);
    }
} // namespace Nyx
