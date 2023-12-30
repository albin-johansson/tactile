// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/document_viewport.hpp"

#include <gtest/gtest.h>

using namespace tactile;

/// \tests tactile::DocumentViewport::DocumentViewport
TEST(DocumentViewport, Defaults)
{
  const DocumentViewport viewport {};

  EXPECT_EQ(viewport.translation().x, 0.0f);
  EXPECT_EQ(viewport.translation().y, 0.0f);
  EXPECT_EQ(viewport.tile_size().x, 32.0f);
  EXPECT_EQ(viewport.tile_size().y, 32.0f);
  EXPECT_EQ(viewport.limits(), nullptr);
}

/// \tests tactile::DocumentViewport::reset_limits
/// \tests tactile::DocumentViewport::limits
TEST(DocumentViewport, ResetLimits)
{
  const Float2 min_offset {-35.3f, 382.3f};
  const Float2 max_offset {843.9f, 486.7f};
  const DocumentViewportLimits limits {min_offset, max_offset};

  DocumentViewport viewport {};

  viewport.reset_limits(limits);
  ASSERT_NE(viewport.limits(), nullptr);
  EXPECT_EQ(viewport.limits()->min_offset, min_offset);
  EXPECT_EQ(viewport.limits()->max_offset, max_offset);

  viewport.reset_limits();
  EXPECT_EQ(viewport.limits(), nullptr);
}

/// \tests tactile::DocumentViewport::translate
/// \tests tactile::DocumentViewport::translation
TEST(DocumentViewport, TranslateWithoutLimits)
{
  DocumentViewport viewport {};
  ASSERT_EQ(viewport.limits(), nullptr);

  viewport.translate(Float2 {0.0f, 0.0f});
  EXPECT_EQ(viewport.translation().x, 0.0f);
  EXPECT_EQ(viewport.translation().y, 0.0f);

  viewport.translate(Float2 {10.0f, 0.0f});
  EXPECT_FLOAT_EQ(viewport.translation().x, 10.0f);
  EXPECT_FLOAT_EQ(viewport.translation().y, 0.0f);

  viewport.translate(Float2 {0.0f, 5.0f});
  EXPECT_FLOAT_EQ(viewport.translation().x, 10.0f);
  EXPECT_FLOAT_EQ(viewport.translation().y, 5.0f);

  viewport.translate(Float2 {-8.0f, 20.0f});
  EXPECT_FLOAT_EQ(viewport.translation().x, 2.0f);
  EXPECT_FLOAT_EQ(viewport.translation().y, 25.0f);
}

/// \tests tactile::DocumentViewport::translate
/// \tests tactile::DocumentViewport::translation
TEST(DocumentViewport, TranslateWithLimits)
{
  const Float2 min_offset {-50.0f, -25.0f};
  const Float2 max_offset {+72.0f, +49.0f};
  const DocumentViewportLimits limits {min_offset, max_offset};

  DocumentViewport viewport {};
  viewport.reset_limits(limits);

  viewport.translate(min_offset);
  EXPECT_FLOAT_EQ(viewport.translation().x, min_offset.x);
  EXPECT_FLOAT_EQ(viewport.translation().y, min_offset.y);

  viewport.translate(Float2 {-1.0f, -1.0f});
  EXPECT_FLOAT_EQ(viewport.translation().x, min_offset.x);
  EXPECT_FLOAT_EQ(viewport.translation().y, min_offset.y);

  viewport.translate(-min_offset);
  EXPECT_FLOAT_EQ(viewport.translation().x, 0.0f);
  EXPECT_FLOAT_EQ(viewport.translation().y, 0.0f);

  viewport.translate(max_offset);
  EXPECT_FLOAT_EQ(viewport.translation().x, max_offset.x);
  EXPECT_FLOAT_EQ(viewport.translation().y, max_offset.y);

  viewport.translate(Float2 {1.0f, 1.0f});
  EXPECT_FLOAT_EQ(viewport.translation().x, max_offset.x);
  EXPECT_FLOAT_EQ(viewport.translation().y, max_offset.y);

  viewport.translate(-max_offset);
  EXPECT_FLOAT_EQ(viewport.translation().x, 0.0f);
  EXPECT_FLOAT_EQ(viewport.translation().y, 0.0f);

  viewport.translate(Float2 {min_offset.x + 1.0f, max_offset.y - 1.0f});
  EXPECT_FLOAT_EQ(viewport.translation().x, min_offset.x + 1.0f);
  EXPECT_FLOAT_EQ(viewport.translation().y, max_offset.y - 1.0f);
}

/// \tests tactile::DocumentViewport::set_tile_size
/// \tests tactile::DocumentViewport::tile_size
TEST(DocumentViewport, SetTileSize)
{
  DocumentViewport viewport {};

  const Float2 tile_size {74.3f, 89.2f};
  viewport.set_tile_size(tile_size);

  EXPECT_EQ(viewport.tile_size(), tile_size);
}
