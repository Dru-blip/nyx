#include "Region.h"
#include "heap/Block.h"
#include "mimalloc.h"


namespace Nyx {
    Region::Region() {
        m_heap = mi_heap_new();
        //TODO: can fail to allocate. handle error.
        m_data = static_cast<uint8_t *>(mi_heap_zalloc(m_heap, Region::Size));

        uint32_t offset = 0;
        for (auto &block: m_blocks) {
            block.set_region(this);
            block.set_data(m_data + offset * Block::Size);
            offset++;
        }
    }
} // namespace Nyx
