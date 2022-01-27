#pragma once

#include <array>        // array
#include <cstddef>      // byte
#include <cstring>      // memset
#include <string>       // string
#include <string_view>  // string_view

#include <tactile_def.hpp>

namespace tactile {

/// \addtogroup utils
/// \{

/**
 * \brief Zeroes the values of a buffer.
 *
 * \tparam Size the size of the buffer.
 *
 * \param buffer the buffer that will be zeroed.
 */
template <usize Size>
void zero_buffer(std::array<char, Size>& buffer)
{
  std::memset(buffer.data(), 0, sizeof buffer);
}

/**
 * \brief Copies a string into a character buffer.
 *
 * \details This function will zero the entire buffer before writing the string
 * into it.
 *
 * \note The string can safely be longer than what the buffer can hold, the
 * buffer will be filled with as many characters as possible.
 *
 * \tparam Size the size of the buffer.
 *
 * \param buffer the buffer that will be modified.
 * \param str the string that will be copied into the buffer.
 */
template <usize Size>
void copy_string_into_buffer(std::array<char, Size>& buffer, const std::string_view str)
{
  zero_buffer(buffer);

  usize index = 0;
  for (const auto ch : str) {
    if (index < buffer.size()) {
      buffer.at(index) = ch;
    }
    else {
      // The string is larger than the buffer, so we are finished
      break;
    }

    ++index;
  }
}

/**
 * \brief Creates a string from a character buffer.
 *
 * \details The function assumes that the "valid" part of the buffer is
 * terminated with a null character ('\0').
 *
 * \tparam Size the size of the buffer.
 *
 * \param buffer the character buffer that contains the string data.
 *
 * \return a string created from the buffer.
 */
template <usize Size>
[[nodiscard]] auto create_string_from_buffer(const std::array<char, Size>& buffer)
    -> std::string
{
  usize index = 0;

  while (index < buffer.size() && buffer.at(index) != '\0') {
    ++index;
  }

  return std::string{buffer.data(), buffer.data() + index};
}

template <usize Size>
[[nodiscard]] auto create_string_view_from_buffer(const std::array<char, Size>& buffer)
    -> std::string_view
{
  usize index = 0;

  while (index < buffer.size() && buffer.at(index) != '\0') {
    ++index;
  }

  return std::string_view{buffer.data(), index};
}

/// \} End of group utils

}  // namespace tactile
