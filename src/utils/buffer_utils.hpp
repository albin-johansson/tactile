#pragma once

#include <array>        // array
#include <cstring>      // memset
#include <string>       // string
#include <string_view>  // string_view

#include "aliases/ints.hpp"

namespace Tactile {

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
void CopyStringIntoBuffer(std::array<char, Size>& buffer,
                          const std::string_view str)
{
  std::memset(buffer.data(), 0, sizeof buffer);

  usize index = 0;
  for (const auto ch : str)
  {
    if (index < buffer.size())
    {
      buffer.at(index) = ch;
    }
    else
    {
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
[[nodiscard]] auto CreateStringFromBuffer(const std::array<char, Size>& buffer)
    -> std::string
{
  usize index = 0;

  while (index < buffer.size() && buffer.at(index) != '\0')
  {
    ++index;
  }

  return std::string{buffer.data(), buffer.data() + index};
}

}  // namespace Tactile
