// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/matrix_index.hpp"

namespace tactile {

auto operator<<(std::ostream& stream, const MatrixIndex& index) -> std::ostream&
{
  stream << '(' << index.row << ';' << index.col << ')';
  return stream;
}

}  // namespace tactile
