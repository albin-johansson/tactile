// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/debug/exception.hpp"

#include <sstream>  // stringstream
#include <utility>  // move

#include <boost/stacktrace.hpp>

namespace tactile {

auto get_stacktrace() -> String
{
  const boost::stacktrace::stacktrace trace;

  std::stringstream stream;
  stream << trace;

  return stream.str();
}

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