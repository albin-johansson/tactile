// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/comp/rename_component_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/component_builder.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(RenameComponentCmd, Constructor)
{
  ASSERT_THROW(RenameComponentCmd(nullptr, make_uuid(), ""), TactileError);
}

TEST(RenameComponentCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto compId = ComponentBuilder {index, "Foo"}.result();

  auto& map = document->get_map();
  auto& bundle = map.get_comps();
  bundle.add(index->at(compId).instantiate());

  RenameComponentCmd cmd {index, compId, "Bar"};
  cmd.redo();

  ASSERT_TRUE(index->contains(compId));
  ASSERT_FALSE(index->contains("Foo"));
  ASSERT_TRUE(index->contains("Bar"));

  ASSERT_EQ(1, bundle.size());
  ASSERT_TRUE(bundle.contains(compId));

  cmd.undo();

  ASSERT_TRUE(index->contains(compId));
  ASSERT_TRUE(index->contains("Foo"));
  ASSERT_FALSE(index->contains("Bar"));

  ASSERT_EQ(1, bundle.size());
  ASSERT_TRUE(bundle.contains(compId));
}

}  // namespace tactile::test
