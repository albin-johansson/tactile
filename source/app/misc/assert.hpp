#pragma once

#include "boost.hpp"
#include "meta/build.hpp"

#if TACTILE_DEBUG

#define TACTILE_ASSERT(Expr) BOOST_ASSERT(Expr)
#define TACTILE_ASSERT_MSG(Expr, Msg) BOOST_ASSERT_MSG(Expr, Msg)

#else

#define TACTILE_ASSERT(Expr)
#define TACTILE_ASSERT_MSG(Expr, Msg)

#endif  // TACTILE_DEBUG

namespace boost {

void assertion_failed_msg(const char* expr,
                          const char* msg,
                          const char* function,
                          const char* file,
                          long line);

inline void assertion_failed(const char* expr,
                             const char* function,
                             const char* file,
                             const long line)
{
  assertion_failed_msg(expr, nullptr, function, file, line);
}

}  // namespace boost
