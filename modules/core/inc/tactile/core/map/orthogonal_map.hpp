// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/map/abstract_map.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * A map variant that uses plain rectangular tiles.
 */
class TACTILE_CORE_API OrthogonalMap final : public AbstractMap {
 public:
  OrthogonalMap(const MatrixExtent& extent, const Int2& tile_size);

  void accept(IMetaContextVisitor& visitor) override;
};

}  // namespace tactile::core
