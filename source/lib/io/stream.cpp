// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "stream.hpp"

#include <ios>      // ios
#include <utility>  // move

namespace tactile {
namespace {

[[nodiscard]] auto open_input_stream_impl(const auto& path, const FileType type)
    -> Maybe<std::ifstream>
{
  const auto flags = (type == FileType::Binary) ? std::ios::in | std::ios::binary  //
                                                : std::ios::in;
  std::ifstream stream {path, flags};

  if (stream.is_open() && stream.good()) {
    return std::move(stream);
  }
  else {
    return nothing;
  }
}

[[nodiscard]] auto open_output_stream_impl(const auto& path, const FileType type)
    -> Maybe<std::ofstream>
{
  const auto flags = (type == FileType::Binary)
                         ? std::ios::out | std::ios::trunc | std::ios::binary
                         : std::ios::out | std::ios::trunc;
  std::ofstream stream {path, flags};

  if (stream.is_open() && stream.good()) {
    return std::move(stream);
  }
  else {
    return nothing;
  }
}

}  // namespace

auto open_input_stream(const char* path, FileType type) -> Maybe<std::ifstream>
{
  return open_input_stream_impl(path, type);
}

auto open_input_stream(const Path& path, FileType type) -> Maybe<std::ifstream>
{
  return open_input_stream_impl(path, type);
}

auto open_output_stream(const char* path, FileType type) -> Maybe<std::ofstream>
{
  return open_output_stream_impl(path, type);
}

auto open_output_stream(const Path& path, FileType type) -> Maybe<std::ofstream>
{
  return open_output_stream_impl(path, type);
}

}  // namespace tactile
