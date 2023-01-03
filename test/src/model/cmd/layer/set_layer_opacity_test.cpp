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

#include "model/cmd/layer/set_layer_opacity.hpp"

#include <doctest/doctest.h>

#include "common/debug/panic.hpp"
#include "core/layer/tile_layer.hpp"

namespace tactile::test {

TEST_SUITE("cmd::SetLayerOpacity")
{
  TEST_CASE("constructor")
  {
    REQUIRE_THROWS_AS(cmd::SetLayerOpacity(nullptr, 1.0f), TactileError);
  }

  TEST_CASE("redo/undo")
  {
    const float old_opacity = 0.4f;
    const float new_opacity = 0.8f;

    auto layer = std::make_shared<TileLayer>();
    layer->set_opacity(old_opacity);

    cmd::SetLayerOpacity cmd {layer, new_opacity};

    cmd.redo();
    REQUIRE(new_opacity == layer->get_opacity());

    cmd.undo();
    REQUIRE(old_opacity == layer->get_opacity());
  }

  TEST_CASE("merge_with")
  {
    auto layer = std::make_shared<TileLayer>();

    cmd::SetLayerOpacity a {layer, 0.8f};
    const cmd::SetLayerOpacity b {layer, 0.6f};
    const cmd::SetLayerOpacity c {layer, 0.4f};

    REQUIRE(a.merge_with(&b));
    REQUIRE(a.merge_with(&c));

    a.redo();
    REQUIRE(0.4f == layer->get_opacity());

    a.undo();
    REQUIRE(1.0f == layer->get_opacity());
  }
}

}  // namespace tactile::test
