// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

class IRenderer;

/**
 * A component that represents a loaded texture resource.
 */
struct CTexture final
{
  /** A raw handle to the underlying API resource. */
  void* raw_handle;

  /** The ID of the associated texture. */
  TextureID id;

  /** The size of the texture. */
  Int2 size;

  /** The path to the file from which the texture was originally loaded. */
  Path path;
};

/**
 * Attempts to load a texture from disk.
 *
 * \param renderer The associated renderer.
 * \param path     The file path to the image file.
 *
 * \return
 * A texture if successful; an error code otherwise.
 */
[[nodiscard]]
auto load_texture(IRenderer& renderer, const Path& path) -> Result<CTexture>;

}  // namespace tactile
