// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

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
#elif __has_cpp_attribute(assume)
  #define TACTILE_ASSERT_MSG(Expr, Msg) [[assume(!!(Expr))]]
  #define TACTILE_ASSERT(Expr) [[assume(!!(Expr))]]
#else
  #define TACTILE_ASSERT_MSG(Expr, Msg) (void) 0
  #define TACTILE_ASSERT(Expr) (void) 0
#endif

namespace tactile {

/**
 * Logs an error message and terminates the program.
 *
 * \details
 *    This function is called by the assert macros if an assertion fails, unless
 *    TACTILE_ENABLE_ASSERTIONS is defined as 0.
 *
 * \param expr The assertion expression.
 * \param msg  The assertion description message.
 * \param file The file that contains the assertion.
 * \param line The file line number of the assertion.
 */
[[noreturn]]
void on_assertion_failed(const char* expr, const char* msg, const char* file, int line);

}  // namespace tactile
