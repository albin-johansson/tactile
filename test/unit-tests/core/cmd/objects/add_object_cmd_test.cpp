// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/object/add_object_cmd.hpp"

#include <gtest/gtest.h>

#include "core/layer/object_layer.hpp"
#include "misc/panic.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(AddObjectCmd, Constructor)
{
  ASSERT_THROW(AddObjectCmd(nullptr, make_uuid(), ObjectType::Rect, {}), TactileError);
}

TEST(AddObjectCmd, RedoUndo)
{
  UUID  layerId;
  auto  document = test::MapBuilder::build().with_object_layer(&layerId).result();
  auto& layer = document->get_map().view_object_layer(layerId);

  AddObjectCmd cmd {document.get(), layerId, ObjectType::Point, {0, 0}};

  cmd.redo();
  ASSERT_EQ(1, layer.object_count());

  auto       object = layer.begin()->second;
  const auto objectId = object->get_uuid();
  ASSERT_TRUE(document->get_contexts().contains(objectId));

  cmd.undo();
  ASSERT_EQ(0, layer.object_count());
  ASSERT_FALSE(document->get_contexts().contains(objectId));
}

}  // namespace tactile::test
