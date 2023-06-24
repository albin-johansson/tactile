/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "model/objects/object_factory.hpp"

#include <doctest/doctest.h>

#include "model/contexts/context_components.hpp"
#include "model/entity_validation.hpp"
#include "model/model_factory.hpp"
#include "model/objects/object_components.hpp"

using namespace tactile;

TEST_SUITE("ObjectFactory")
{
  TEST_CASE("create_object")
  {
    auto model = sys::create_model(BackendAPI::Null);

    const auto object_entity = sys::create_object(model, ObjectType::Ellipse);
    REQUIRE(sys::is_object_entity(model, object_entity));

    const auto& object = model.get<Object>(object_entity);
    const auto& object_context = model.get<Context>(object_entity);

    CHECK(object.type == ObjectType::Ellipse);
    CHECK(object.position == Float2 {});
    CHECK(object.size == Float2 {});
    CHECK(object.visible);
    CHECK(object.tag.empty());
    CHECK(!object.meta_id.has_value());

    CHECK(object_context.name == "Object");
    CHECK(object_context.props.empty());
    CHECK(object_context.comps.empty());
  }
}