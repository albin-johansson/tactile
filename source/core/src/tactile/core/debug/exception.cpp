// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/exception.hpp"

#include <utility>  // move

#include "tactile/core/debug/stacktrace.hpp"

namespace tactile {

Exception::Exception(std::string message) :
  mMessage {std::move(message)},
  mTrace {get_stacktrace()}
{}

auto Exception::what() const noexcept -> const char*
{
  return mMessage.c_str();
}

auto Exception::trace() const -> const std::string&
{
  return mTrace;
}

}  // namespace tactile
