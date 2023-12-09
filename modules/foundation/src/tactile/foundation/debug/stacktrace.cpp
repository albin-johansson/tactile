// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/debug/stacktrace.hpp"

#include <sstream>  // stringstream

#include <boost/stacktrace.hpp>

namespace tactile {

auto get_stacktrace() -> String
{
  const boost::stacktrace::stacktrace trace {};

  std::stringstream stream;
  stream << trace;

  return stream.str();
}

}  // namespace tactile
