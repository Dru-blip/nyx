#pragma once

#include <vector>
#include "Region.h"

namespace Nyx {
    class RegionAllocator {
    public:
        RegionAllocator();
        Region *acquire_region();

    private:
        mi_heap_t *m_region_heap;
        std::vector<Region *> m_regions;
    };
} // namespace Nyx
