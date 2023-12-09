// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/debug/exception.hpp"

#include <utility>  // move

#include "tactile/foundation/debug/stacktrace.hpp"

namespace tactile {

Exception::Exception(String message)
  : mMessage {std::move(message)},
    mTrace {get_stacktrace()}
{}

auto Exception::what() const noexcept -> const char*
{
  return mMessage.c_str();
}

auto Exception::get_trace() const -> const String&
{
  return mTrace;
}

}  // namespace tactile