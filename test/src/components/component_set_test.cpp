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

#include <doctest/doctest.h>

#include "components/component.hpp"
#include "model/systems/model_system.hpp"

using namespace tactile;

TEST_SUITE("ComponentSet")
{
  TEST_CASE("Defaults")
  {
    const ComponentSet set;
    CHECK(set.definitions.empty());
  }

  TEST_CASE("has_component")
  {
    Model model;
    sys::init_model(model, BackendAPI::Null);

    ComponentSet set;
    CHECK(!set.has_component(kNullEntity));

    const auto entity_a = model.create_entity();
    const auto entity_b = model.create_entity();
    const auto entity_c = model.create_entity();

    set.definitions.push_back(entity_a);
    set.definitions.push_back(entity_b);

    CHECK(set.has_component(entity_a));
    CHECK(set.has_component(entity_b));
    CHECK(!set.has_component(entity_c));
  }
}