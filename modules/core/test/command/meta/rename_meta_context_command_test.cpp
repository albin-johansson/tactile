// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/meta/rename_meta_context_command.hpp"

#include <utility>  // move

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/object.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/map/orthogonal_map.hpp"

using namespace tactile;

class RenameMetaContextCommandTest : public testing::Test {
 protected:
  RenameMetaContextCommandTest()
    : mMapDocument {make_unique<OrthogonalMap>(MatrixExtent {5, 5}, Int2 {32, 32})},
      mObjectLayer {make_shared<ObjectLayer>()}
  {
    auto& map = mMapDocument.map();
    map.root_layer().append_layer(mObjectLayer);
  }

  MapDocument mMapDocument;
  Shared<ObjectLayer> mObjectLayer;
};

/// \tests tactile::RenameMetaContextCommand::redo
/// \tests tactile::RenameMetaContextCommand::undo
TEST_F(RenameMetaContextCommandTest, RedoUndo)
{
  auto& object = mObjectLayer->emplace_object(ObjectType::kPoint);
  object.meta().set_name("ABC");

  const auto object_uuid = object.meta().uuid();

  RenameMetaContextCommand command {&mMapDocument, object_uuid, "DEF"};
  EXPECT_EQ(object.meta().name(), "ABC");

  command.redo();
  EXPECT_EQ(object.meta().name(), "DEF");

  command.undo();
  EXPECT_EQ(object.meta().name(), "ABC");
}

/// \tests tactile::RenameMetaContextCommand::merge_with
TEST_F(RenameMetaContextCommandTest, MergeWith)
{
  auto& rect = mObjectLayer->emplace_object(ObjectType::kRect);
  auto& ellipse = mObjectLayer->emplace_object(ObjectType::kRect);

  ellipse.meta().set_name("start");

  const auto rect_uuid = rect.meta().uuid();
  const auto ellipse_uuid = ellipse.meta().uuid();

  RenameMetaContextCommand ellipse_foo_command {&mMapDocument, ellipse_uuid, "foo"};
  const RenameMetaContextCommand rect_bar_command {&mMapDocument, rect_uuid, "bar"};
  const RenameMetaContextCommand ellipse_bar_command {&mMapDocument, ellipse_uuid, "bar"};

  // Not the same target
  EXPECT_FALSE(ellipse_foo_command.merge_with(&rect_bar_command));

  // "foo" -> "bar"
  EXPECT_TRUE(ellipse_foo_command.merge_with(&ellipse_bar_command));

  // "start" -> "bar"
  ellipse_foo_command.redo();
  EXPECT_EQ(ellipse.meta().name(), "bar");

  // "bar" -> "start"
  ellipse_foo_command.undo();
  EXPECT_EQ(ellipse.meta().name(), "start");
}
