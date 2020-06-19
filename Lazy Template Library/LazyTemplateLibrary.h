#pragma once

#include <assert.h>

#define BEGIN_LAZY_NAMESPACE namespace lzy {
#define END_LAZY_NAMESPACE }

#define lzy_assert(condition, message) assert((!(condition) && message))