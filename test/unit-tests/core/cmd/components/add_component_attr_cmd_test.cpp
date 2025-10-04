// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/comp/add_component_attr_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/component_builder.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(AddComponentAttrCmd, Constructor)
{
  ASSERT_THROW(AddComponentAttrCmd(nullptr, make_uuid(), ""), TactileError);
}

TEST(AddComponentAttrCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto compId = ComponentBuilder {index, "Demo"}.result();
  auto&      definition = index->at(compId);
  ASSERT_TRUE(definition.empty());

  auto& map = document->get_map();
  auto& bundle = map.get_comps();

  bundle.add(definition.instantiate());
  ASSERT_TRUE(bundle.contains(compId));
  ASSERT_TRUE(bundle.at(compId).empty());

  AddComponentAttrCmd cmd {document.get(), compId, "attr"};

  cmd.redo();
  ASSERT_EQ(1, definition.size());
  ASSERT_TRUE(definition.has_attr("attr"));
  ASSERT_EQ(definition.get_attr("attr"), bundle.at(compId).get_attr("attr"));

  cmd.undo();
  ASSERT_TRUE(definition.empty());
  ASSERT_FALSE(definition.has_attr("attr"));
  ASSERT_THROW(bundle.at(compId).get_attr("attr"), TactileError);
}

}  // namespace tactile::test
