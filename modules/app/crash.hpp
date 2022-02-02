#pragma once

#include "boost.hpp"

namespace tactile {

void dump_crash_info(const boost::stacktrace::stacktrace& trace);

}  // namespace tactile