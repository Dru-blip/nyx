
#include "BlockAllocator.h"
#include <cassert>
#include "heap/Block.h"

#include "heap/Region.h"

namespace Nyx {
    Block *BlockAllocator::acquire_block() {
        if (m_free_list.empty()) {
            grow();
        }

        Block *block = m_free_list.front();
        m_free_list.pop();

        return block;
    }

    void BlockAllocator::grow() {
        Region *region = m_region_allocator.acquire_region();
        assert(region != nullptr);
        for (auto &block: region->get_blocks()) {
            m_free_list.push(&block);
        }
    }
} // namespace Nyx
