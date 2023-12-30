// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/document_viewport.hpp"

#include <glm/common.hpp>

#include "tactile/foundation/debug/assert.hpp"

namespace tactile {

void DocumentViewport::reset_limits(const Maybe<DocumentViewportLimits>& limits)
{
  mLimits = limits;
}

void DocumentViewport::translate(const Float2 delta)
{
  mTranslation += delta;

  if (mLimits.has_value()) {
    mTranslation = glm::max(mTranslation, mLimits->min_offset);
    mTranslation = glm::min(mTranslation, mLimits->max_offset);
  }
}

void DocumentViewport::set_tile_size(const Float2 tile_size)
{
  TACTILE_ASSERT(tile_size.x > 0.0f);
  TACTILE_ASSERT(tile_size.y > 0.0f);
  mTileSize = tile_size;
}

auto DocumentViewport::translation() const -> const Float2&
{
  return mTranslation;
}

auto DocumentViewport::tile_size() const -> const Float2&
{
  return mTileSize;
}

auto DocumentViewport::limits() const -> const DocumentViewportLimits*
{
  return mLimits.has_value() ? &*mLimits : nullptr;
}

}  // namespace tactile
