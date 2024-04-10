// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/matrix_extent.hpp"

namespace tactile {

auto operator<<(std::ostream& stream,
                const MatrixExtent& extent) -> std::ostream&
{
  stream << '(' << extent.rows << ';' << extent.cols << ')';
  return stream;
}

}  // namespace tactile
