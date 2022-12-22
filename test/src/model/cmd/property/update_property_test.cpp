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

#include "model/cmd/property/update_property.hpp"

#include <centurion/color.hpp>
#include <gtest/gtest.h>

#include "core/helpers/map_builder.hpp"
#include "debug/panic.hpp"

namespace tactile::test {

TEST(UpdateProperty, Constructor)
{
  ASSERT_THROW(cmd::UpdateProperty(nullptr, "", 0), TactileError);
}

TEST(UpdateProperty, RedoUndo)
{
  auto document = MapBuilder::build().result();
  auto map = document->get_map_ptr();

  auto& props = map->get_ctx().props();
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

  const Color cyan {0, 0xFF, 0xFF};
  const Color azure {0xF0, 0xFF, 0xFF};
  const Color gold {0xFF, 0xD7, 0};
  const Color violet {0xEE, 0x82, 0xEE};

  auto& props = map->get_ctx().props();
  props.add("color", cyan);

  cmd::UpdateProperty a {map, "color", azure};
  const cmd::UpdateProperty b {map, "color", gold};
  const cmd::UpdateProperty c {map, "color", violet};

  ASSERT_TRUE(a.merge_with(&b));
  ASSERT_TRUE(a.merge_with(&c));

  a.redo();
  ASSERT_EQ(violet, props.at("color").as_color());

  a.undo();
  ASSERT_EQ(cyan, props.at("color").as_color());
}

}  // namespace tactile::test
