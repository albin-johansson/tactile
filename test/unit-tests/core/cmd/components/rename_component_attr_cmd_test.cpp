// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/comp/rename_component_attr_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/component_builder.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(RenameComponentAttrCmd, Constructor)
{
  ASSERT_THROW(RenameComponentAttrCmd(nullptr, make_uuid(), "", ""), TactileError);
}

TEST(RenameComponentAttrCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto compId = ComponentBuilder {index, "Demo"}  //
                          .with_attr("Foo", 123)
                          .result();

  auto& map = document->get_map();
  auto& bundle = map.get_comps();
  bundle.add(index->at(compId).instantiate());

  RenameComponentAttrCmd cmd {document.get(), compId, "Foo", "Bar"};
  cmd.redo();

  {
    const auto& def = index->at(compId);
    const auto& comp = bundle.at(compId);

    ASSERT_FALSE(def.has_attr("Foo"));
    ASSERT_FALSE(comp.has_attr("Foo"));

    ASSERT_TRUE(def.has_attr("Bar"));
    ASSERT_TRUE(comp.has_attr("Bar"));

    ASSERT_EQ(123, def.get_attr("Bar"));
    ASSERT_EQ(123, comp.get_attr("Bar"));
  }

  cmd.undo();

  {
    const auto& def = index->at(compId);
    const auto& comp = bundle.at(compId);

    ASSERT_TRUE(def.has_attr("Foo"));
    ASSERT_TRUE(comp.has_attr("Foo"));

    ASSERT_FALSE(def.has_attr("Bar"));
    ASSERT_FALSE(comp.has_attr("Bar"));

    ASSERT_EQ(123, def.get_attr("Foo"));
    ASSERT_EQ(123, comp.get_attr("Foo"));
  }
}

}  // namespace tactile::test
