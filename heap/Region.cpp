

#include "Region.h"
#include "heap/Block.h"
#include "mimalloc.h"


namespace Nyx {
    Region::Region() {
        m_heap = mi_heap_new();
        m_data = static_cast<uint8_t *>(mi_heap_zalloc(m_heap, Region::Size));

        for (auto &block: m_blocks) {
            block.set_region(this);
        }
    }
} // namespace Nyx
