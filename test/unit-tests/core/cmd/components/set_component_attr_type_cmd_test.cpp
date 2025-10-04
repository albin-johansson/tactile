// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/comp/set_component_attr_type_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/component_builder.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

namespace tactile::test {

TEST(SetComponentAttrTypeCmd, Constructor)
{
  ASSERT_THROW(SetComponentAttrTypeCmd(nullptr, make_uuid(), "", AttributeType::String),
               TactileError);
}

TEST(SetComponentAttrTypeCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto compId = ComponentBuilder {index, "Demo"}  //
                          .with_attr("Attr", true)
                          .with_attr("Attr2", 938)
                          .with_attr("Attr3", 94.3f)
                          .result();

  auto& map = document->get_map();
  auto& bundle = map.get_comps();
  bundle.add(index->at(compId).instantiate());

  SetComponentAttrTypeCmd cmd {document.get(), compId, "Attr", AttributeType::Int};
  cmd.redo();

  {
    const auto& def = index->at(compId);
    const auto& comp = bundle.at(compId);

    ASSERT_EQ(AttributeType::Int, def.get_attr("Attr").type());
    ASSERT_EQ(AttributeType::Int, comp.get_attr("Attr").type());

    ASSERT_EQ(0, def.get_attr("Attr").as_int());
    ASSERT_EQ(0, comp.get_attr("Attr").as_int());
  }

  cmd.undo();

  {
    const auto& def = index->at(compId);
    const auto& comp = bundle.at(compId);

    ASSERT_EQ(AttributeType::Bool, def.get_attr("Attr").type());
    ASSERT_EQ(AttributeType::Bool, comp.get_attr("Attr").type());

    ASSERT_TRUE(def.get_attr("Attr").as_bool());
    ASSERT_TRUE(comp.get_attr("Attr").as_bool());
  }
}

}  // namespace tactile::test
