
#include "RegionAllocator.h"
#include "heap/Region.h"
#include "mimalloc.h"


namespace Nyx {
    RegionAllocator::RegionAllocator() { m_region_heap = mi_heap_new(); }

    Region *RegionAllocator::acquire_region() {
        void *mem = mi_heap_zalloc(m_region_heap, sizeof(Region));
        Region *region = new (mem) Region();
        m_regions.emplace_back(region);
        return region;
    }
} // namespace Nyx
