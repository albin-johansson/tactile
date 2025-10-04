// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/comp/duplicate_component_attr_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/component_builder.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(DuplicateComponentAttrCmd, Constructor)
{
  ASSERT_THROW(DuplicateComponentAttrCmd(nullptr, make_uuid(), ""), TactileError);
}

TEST(DuplicateComponentAttrCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto compId = ComponentBuilder {index, "Demo"}  //
                          .with_attr("Attr", 42)
                          .result();

  auto& map = document->get_map();
  auto& bundle = map.get_comps();
  bundle.add(index->at(compId).instantiate());

  DuplicateComponentAttrCmd cmd {document.get(), compId, "Attr"};
  cmd.redo();

  {
    const auto& def = index->at(compId);
    const auto& comp = bundle.at(compId);

    ASSERT_EQ(2, def.size());
    ASSERT_TRUE(def.has_attr("Attr"));
    ASSERT_TRUE(def.has_attr("Attr (1)"));
    ASSERT_EQ(def.get_attr("Attr"), def.get_attr("Attr (1)"));

    ASSERT_EQ(2, comp.size());
    ASSERT_TRUE(comp.has_attr("Attr"));
    ASSERT_TRUE(comp.has_attr("Attr (1)"));
    ASSERT_EQ(comp.get_attr("Attr"), comp.get_attr("Attr (1)"));
  }

  cmd.undo();

  {
    const auto& def = index->at(compId);
    const auto& comp = bundle.at(compId);

    ASSERT_EQ(1, def.size());
    ASSERT_TRUE(def.has_attr("Attr"));
    ASSERT_FALSE(def.has_attr("Attr (1)"));

    ASSERT_EQ(1, comp.size());
    ASSERT_TRUE(comp.has_attr("Attr"));
    ASSERT_FALSE(comp.has_attr("Attr (1)"));
  }
}

}  // namespace tactile::test
