// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// \addtogroup Texture
/// \{

/**
 * A component that represents a loaded texture resource.
 */
struct CTexture final
{
  /** The UUID of the associated texture. */
  UUID texture_uuid;

  /** The size of the texture. */
  Int2 size;

  /** The path to the file from which the texture was originally loaded. */
  Path path;
};

/// \}

}  // namespace tactile
