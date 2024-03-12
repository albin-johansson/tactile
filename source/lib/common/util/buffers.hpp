// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/span.hpp"
#include "tactile/base/container/string.hpp"

namespace tactile {

/// Zeroes the values of a buffer.
void zero_buffer(Span<char> buffer);

/// Copies a string into a character buffer.
///
/// This function will zero the entire buffer before writing the string into it.
///
/// Note, the string can safely be longer than what the buffer can hold, the buffer will
/// be filled with as many characters as possible.
///
/// \param buffer the buffer that will be modified.
/// \param str the string that will be copied into the buffer.
void copy_string_into_buffer(Span<char> buffer, StringView str);

/// Creates a string from a character buffer.
///
/// The function assumes that the "valid" part of the buffer is terminated with a null
/// character ('\0').
///
/// \param buffer the character buffer that contains the string data.
///
/// \return a string created from the buffer.
[[nodiscard]] auto create_string_from_buffer(Span<const char> buffer) -> String;

/// Creates a string view from a character buffer.
///
/// The function assumes that the "valid" part of the buffer is terminated with a null
/// character ('\0').
///
/// Beware of the lifetime of the pointed-to buffer! The buffer must not be destroyed
/// before the returned string view. This function is useful to obtain a temporary
/// string-like object to perform queries on, use the string overload if you need to store
/// the returned string.
///
/// \param buffer the character buffer that contains the string data.
///
/// \return a string created from the buffer.
[[nodiscard]] auto create_string_view_from_buffer(Span<const char> buffer) -> StringView;

}  // namespace tactile
