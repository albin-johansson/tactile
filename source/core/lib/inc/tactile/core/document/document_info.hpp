// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>  // string

#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile::core {

/**
 * A context component that provides information about the associated document.
 */
struct CDocumentInfo final
{
  /** The human-readable name of the document. */
  std::string name;

  /** The "root" entity, basically a map or a tileset. */
  EntityID root;

  /** The active meta context, should always be valid. */
  EntityID active_context;

  /** The logical size of associated tiles. */
  Int2 tile_size;
};

}  // namespace tactile::core
