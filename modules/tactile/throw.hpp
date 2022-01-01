#pragma once

#include <boost/exception/all.hpp>
#include <boost/stacktrace.hpp>

namespace Tactile {

using TracedError = boost::error_info<struct ErrorInfoTag, boost::stacktrace::stacktrace>;

template <typename Exception>
[[noreturn]] void ThrowTraced(const Exception& e)
{
  throw boost::enable_error_info(e) << TracedError{boost::stacktrace::stacktrace()};
}

}  // namespace Tactile
