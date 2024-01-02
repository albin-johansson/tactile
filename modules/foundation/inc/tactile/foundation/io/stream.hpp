// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <exception>  // exception
#include <fstream>    // ifstream, ofstream
#include <iomanip>    // setw
#include <ios>        // ios

#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Provides input stream options.
 */
struct StreamFromFileOptions final {
  bool binary_mode : 1 {false};  ///< Set to true to read binary files.
};

/**
 * Attempts to stream a file from disk into a container object.
 *
 * \tparam T A type that provides an overloaded operator>> that accepts an istream.
 *
 * \param path    The path to the source file.
 * \param options The read options.
 *
 * \return
 *    The parsed content, or nothing if an error occurred.
 */
template <typename T>
[[nodiscard]] auto stream_from_file(const FilePath& path,
                                    const StreamFromFileOptions& options = {}) -> Maybe<T>
{
  std::ios::openmode stream_flags = std::ios::in;
  if (options.binary_mode) {
    stream_flags |= std::ios::binary;
  }

  std::ifstream stream {path, stream_flags};  // NOLINT: shall not be const.
  if (!stream.good()) {
    TACTILE_LOG_ERROR("Could not open file {}", path.string());
    return kNone;
  }

  try {
    T object {};
    stream >> object;
    return object;
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_ERROR("Could not read from file {}: {}", path.string(), ex.what());
  }

  return kNone;
}

/**
 * Provides output stream options.
 */
struct StreamToFileOptions final {
  int indentation {0};       ///< The amount of indentation to use (only for text files).
  bool binary_mode {false};  ///< Set to true to write binary files.
};

/**
 * Attempts to stream an object into a file on disk.
 *
 * \tparam T A type that provides an overloaded operator<< that accepts an ostream.
 *
 * \param content The content that will be written to the file.
 * \param path    The path to the destination file.
 * \param options The write options.
 *
 * \return
 *    An error code if something goes wrong.
 */
template <typename T>
auto stream_to_file(const T& content,
                    const FilePath& path,
                    const StreamToFileOptions& options = {}) -> Result<void>
{
  std::ios::openmode stream_flags = std::ios::out | std::ios::trunc;
  if (options.binary_mode) {
    stream_flags |= std::ios::binary;
  }

  std::ofstream stream {path, stream_flags};
  if (!stream.good()) {
    TACTILE_LOG_ERROR("Could not open file {}", path.string());
    return unexpected(make_generic_error(GenericError::kInvalidFile));
  }

  try {
    if (!options.binary_mode && options.indentation > 0) {
      stream << std::setw(options.indentation);
    }

    stream << content;
    return kOK;
  }
  catch (const std::exception& ex) {
    TACTILE_LOG_ERROR("Could not write to file {}: {}", path.string(), ex.what());
    return unexpected(make_generic_error(GenericError::kIOError));
  }
}

}  // namespace tactile