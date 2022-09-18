/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "core/cmd/property/update_property.hpp"

#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "misc/panic.hpp"

namespace tactile::test {

TEST(UpdateProperty, Constructor)
{
  ASSERT_THROW(cmd::UpdateProperty(nullptr, "", 0), TactileError);
}

TEST(UpdateProperty, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  auto& props = map->ctx().props();
  props.add("int", 10);

  cmd::UpdateProperty cmd {map, "int", 20};

  cmd.redo();
  ASSERT_EQ(20, props.at("int"));

  cmd.undo();
  ASSERT_EQ(10, props.at("int"));
}

TEST(UpdateProperty, MergeSupport)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  auto& props = map->ctx().props();
  props.add("color", cen::colors::cyan);

  cmd::UpdateProperty a {map, "color", cen::colors::azure};
  const cmd::UpdateProperty b {map, "color", cen::colors::gold};
  const cmd::UpdateProperty c {map, "color", cen::colors::violet};

  ASSERT_TRUE(a.merge_with(&b));
  ASSERT_TRUE(a.merge_with(&c));

  a.redo();
  ASSERT_EQ(cen::colors::violet, props.at("color").as_color());

  a.undo();
  ASSERT_EQ(cen::colors::cyan, props.at("color").as_color());
}

}  // namespace tactile::test
