#ifndef RUNE_INDEX_TO_MATRIX_HPP_
#define RUNE_INDEX_TO_MATRIX_HPP_

#include <concepts>  // integral

namespace rune {

/// \addtogroup math
/// \{

template <std::integral T>
struct matrix_position final
{
  T row{};
  T column{};
};

template <std::integral T = int>
[[nodiscard]] constexpr auto index_to_matrix(const T index, const T nColumns) noexcept
    -> matrix_position<T>
{
  return {.row = index / nColumns, .column = index % nColumns};
}

/// \} End of group math

}  // namespace rune

#endif  // RUNE_INDEX_TO_MATRIX_HPP_
