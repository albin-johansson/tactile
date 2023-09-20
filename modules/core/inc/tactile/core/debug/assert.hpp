// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/prelude.hpp"

#ifndef TACTILE_ENABLE_ASSERTIONS
  #if TACTILE_DEBUG
    #define TACTILE_ENABLE_ASSERTIONS 1
  #else
    #define TACTILE_ENABLE_ASSERTIONS 0
  #endif
#endif

#if TACTILE_ENABLE_ASSERTIONS
  #define TACTILE_ASSERT_MSG(Expr, Msg) \
    (!!(Expr) ? ((void) 0)              \
              : tactile::on_assertion_failed(#Expr, (Msg), __FILE__, __LINE__))
  #define TACTILE_ASSERT(Expr) TACTILE_ASSERT_MSG(Expr, "?")
#else
  #define TACTILE_ASSERT_MSG(Expr, Msg) (void) 0
  #define TACTILE_ASSERT(Expr) (void) 0
#endif

namespace tactile {

/**
 * \brief Logs an error message and terminates the program.
 *
 * \details This function is called by the assert macros if an assertion fails, unless
 *          TACTILE_ENABLE_ASSERTIONS is defined as 0.
 *
 * \param expr the assertion expression.
 * \param msg  the assertion description message.
 * \param file the file that contains the assertion.
 * \param line the file line number of the assertion.
 */
[[noreturn]]
TACTILE_CORE_API void on_assertion_failed(const char* expr,
                                          const char* msg,
                                          const char* file,
                                          int64 line);

}  // namespace tactile
