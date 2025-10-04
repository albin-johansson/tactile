// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/comp/component_index.hpp"

#include <gtest/gtest.h>

#include "core/comp/component_definition.hpp"
#include "misc/panic.hpp"

namespace tactile::test {

TEST(ComponentIndex, Defaults)
{
  const ComponentIndex index;
  ASSERT_EQ(0, index.size());
}

TEST(ComponentIndex, DefineComp)
{
  ComponentIndex index;
  const auto     id = index.define_comp("position");

  const auto& def = index.at(id);
  ASSERT_EQ("position", def.get_name());
  ASSERT_EQ(id, def.get_uuid());
  ASSERT_TRUE(def.empty());

  ASSERT_TRUE(index.contains("position"));
  ASSERT_EQ(1, index.size());

  ASSERT_THROW(index.define_comp("position"), TactileError);
}

TEST(ComponentIndex, RemoveComp)
{
  ComponentIndex index;
  ASSERT_THROW(index.remove_comp(make_uuid()), TactileError);

  const auto id = index.define_comp("foo");
  ASSERT_EQ(1, index.size());
  ASSERT_TRUE(index.contains("foo"));

  index.remove_comp(id);

  ASSERT_EQ(0, index.size());
  ASSERT_FALSE(index.contains("foo"));
}

TEST(ComponentIndex, RenameComp)
{
  ComponentIndex index;
  ASSERT_THROW(index.rename_comp(make_uuid(), "foo"), TactileError);

  const auto id = index.define_comp("foo");

  ASSERT_TRUE(index.contains("foo"));
  ASSERT_FALSE(index.contains("zoo"));

  index.rename_comp(id, "zoo");

  ASSERT_FALSE(index.contains("foo"));
  ASSERT_TRUE(index.contains("zoo"));

  index.define_comp("woo");
  ASSERT_THROW(index.rename_comp(id, "woo"), TactileError);
}

}  // namespace tactile::test
