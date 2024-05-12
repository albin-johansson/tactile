// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

/// \addtogroup Texture
/// \{

/**
 * A component that represents a loaded texture resource.
 */
struct CTexture final
{
  /** The ID of the associated texture. */
  TextureID texture_id;

  /** The size of the texture. */
  Int2 size;

  /** The path to the file from which the texture was originally loaded. */
  Path path;
};

/// \}

}  // namespace tactile
