// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/cmd/comp/undef_component_cmd.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"
#include "unit-tests/core/helpers/component_builder.hpp"
#include "unit-tests/core/helpers/map_builder.hpp"

using namespace std::string_literals;

namespace tactile::test {

TEST(UndefComponentCmd, Constructor)
{
  ASSERT_THROW(UndefComponentCmd(nullptr, make_uuid()), TactileError);
}

TEST(UndefComponentCmd, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto index = document->get_component_index();

  const auto compId = ComponentBuilder {index, "Demo"}  //
                          .with_attr("foo", "bar"s)
                          .with_attr("threshold", 1.5f)
                          .result();

  auto& map = document->get_map();
  auto& bundle = map.get_comps();

  bundle.add(index->at(compId).instantiate());
  bundle.at(compId).update_attr("foo", "abc"s);
  bundle.at(compId).update_attr("threshold", 8.9f);

  UndefComponentCmd cmd {document.get(), compId};
  cmd.redo();

  ASSERT_FALSE(index->contains("Demo"));
  ASSERT_FALSE(index->contains(compId));

  ASSERT_FALSE(bundle.contains(compId));
  ASSERT_TRUE(bundle.empty());

  cmd.undo();

  ASSERT_TRUE(index->contains("Demo"));
  ASSERT_TRUE(index->contains(compId));

  ASSERT_EQ("bar", index->at(compId).get_attr("foo").as_string());
  ASSERT_EQ(1.5f, index->at(compId).get_attr("threshold").as_float());

  ASSERT_EQ("abc", bundle.at(compId).get_attr("foo").as_string());
  ASSERT_EQ(8.9f, bundle.at(compId).get_attr("threshold").as_float());
}

}  // namespace tactile::test
