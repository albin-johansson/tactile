#pragma once

#include <boost/stacktrace.hpp>

namespace tactile {

void dump_crash_info(const boost::stacktrace::stacktrace& trace);

}  // namespace tactile