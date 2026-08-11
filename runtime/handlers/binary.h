
#include "runtime/ops/Integer.h"

namespace Nyx::Handlers {
    using namespace Nyx::Ops;

    static inline Value handle_add(const Value a, const Value b) { return Ops::Integer::add(a, b); }

    static inline Value handle_sub(const Value a, const Value b) { return Ops::Integer::sub(a, b); }

    static inline Value handle_mul(const Value a, const Value b) { return Ops::Integer::mul(a, b); }

    static inline Value handle_div(const Value a, const Value b) { return Ops::Integer::div(a, b); }

    static inline Value handle_lt(const Value a, const Value b) { return Ops::Integer::lt(a, b); }

    static inline Value handle_lte(const Value a, const Value b) { return Ops::Integer::lte(a, b); }

    static inline Value handle_gt(const Value a, const Value b) { return Ops::Integer::gt(a, b); }

    static inline Value handle_gte(const Value a, const Value b) { return Ops::Integer::gte(a, b); }

    static inline Value handle_eq(const Value a, const Value b) { return Ops::Integer::eq(a, b); }

    static inline Value handle_neq(const Value a, const Value b) { return Ops::Integer::neq(a, b); }

    static inline Value handle_neg(const Value a) { return Ops::Integer::neg(a); }

    static inline Value handle_not(const Value a) { return Ops::Integer::not_(a); }
} // namespace Nyx::Handlers
