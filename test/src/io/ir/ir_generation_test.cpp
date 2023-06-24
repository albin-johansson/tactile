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

#include "io/ir/ir_generation.hpp"

#include <doctest/doctest.h>

#include "helpers/component_builder.hpp"
#include "helpers/layer_builder.hpp"
#include "model/components/component_factory.hpp"
#include "model/components/component_set_system.hpp"
#include "model/contexts/attached_component_system.hpp"
#include "model/layers/layer_components.hpp"
#include "model/model_factory.hpp"
#include "model/objects/object_components.hpp"
#include "model/objects/object_factory.hpp"

using namespace tactile;

TEST_SUITE("IrGeneration")
{
  TEST_CASE("convert_context_to_ir")
  {
    auto model = sys::create_model(BackendAPI::Null);

    const auto component_set_entity = sys::create_component_set(model);
    const auto component_entity = ComponentBuilder {model, component_set_entity}
                                      .with_name("Bundle")
                                      .with_attribute("X", 10)
                                      .with_attribute("Y", 20)
                                      .build();

    const auto layer_entity = LayerBuilder {model}
                                  .as_tile_layer({5, 5})
                                  .with_id(42)
                                  .with_name("Test")
                                  .with_property("A", 123)
                                  .with_property("B", 12.3f)
                                  .with_property("C", true)
                                  .with_property("D", Color {0x10, 0x20, 0x30})
                                  .with_property("E", ObjectRef {42})
                                  .with_property("F", String {"hello"})
                                  .with_property("G", Path {"foo/bar"})
                                  .with_component(component_entity)
                                  .build();

    const auto& layer_context = model.get<Context>(layer_entity);

    ContextIR ir_context;
    convert_context_to_ir(model, component_set_entity, layer_context, ir_context);

    CHECK(ir_context.properties == layer_context.props);
    CHECK(ir_context.components.size() == layer_context.comps.size());
    CHECK(ir_context.components.size() == 1);

    const auto& ir_component = ir_context.components.at("Bundle");
    CHECK(ir_component.size() == 2);
    CHECK(ir_component.at("X") == 10);
    CHECK(ir_component.at("Y") == 20);
  }

  TEST_CASE("convert_object_to_ir")
  {
    auto model = sys::create_model(BackendAPI::Null);

    const auto component_set_entity = sys::create_component_set(model);
    const auto object_entity = sys::create_object(model, ObjectType::Ellipse);

    auto& object_context = model.get<Context>(object_entity);
    object_context.name = "Ellipse";
    object_context.props["1"] = 1;

    auto& object = model.get<Object>(object_entity);
    object.meta_id = 42;
    object.position = Float2 {-31, 184};
    object.size = Float2 {28, 93};
    object.tag = "Tag";
    object.visible = false;

    ObjectIR ir_object;
    convert_object_to_ir(model, component_set_entity, object_entity, ir_object);

    CHECK(object.type == ir_object.type);
    CHECK(object.position == ir_object.pos);
    CHECK(object.size == ir_object.size);
    CHECK(object.size == ir_object.size);
    CHECK(object.visible == ir_object.visible);
    CHECK(object.tag == ir_object.tag);
    CHECK(object_context.name == ir_object.name);
    CHECK(object_context.props == ir_object.context.properties);
    CHECK(ir_object.context.components.empty());
  }

  TEST_CASE("convert_tile_layer_to_ir")
  {
    auto model = sys::create_model(BackendAPI::Null);

    const auto layer_entity = LayerBuilder {model}
                                  .as_tile_layer({10, 9})
                                  .set_tile({5, 2}, 938)
                                  .set_tile({1, 8}, 25)
                                  .build();

    const auto& tile_layer = model.get<TileLayer>(layer_entity);

    TileLayerIR ir_tile_layer;
    convert_tile_layer_to_ir(model, layer_entity, ir_tile_layer);

    CHECK(ir_tile_layer.extent == TileExtent {10, 9});
    CHECK(ir_tile_layer.tiles.at(5).at(2) == 938);
    CHECK(ir_tile_layer.tiles.at(1).at(8) == 25);
    CHECK(tile_layer.tiles == ir_tile_layer.tiles);
  }

  TEST_CASE("convert_object_layer_to_ir")
  {
    auto model = sys::create_model(BackendAPI::Null);
    const auto component_set_entity = sys::create_component_set(model);

    Entity point_entity = kNullEntity;
    Entity ellipse_entity = kNullEntity;
    const auto layer_entity = LayerBuilder {model}
                                  .as_object_layer()
                                  .with_object(1, ObjectType::Point, &point_entity)
                                  .with_object(2, ObjectType::Ellipse, &ellipse_entity)
                                  .build();

    const auto& object_layer = model.get<ObjectLayer>(layer_entity);

    auto& point_object = model.get<Object>(point_entity);
    point_object.meta_id = 123;

    auto& ellipse_object = model.get<Object>(ellipse_entity);
    ellipse_object.meta_id = 124;

    ObjectLayerIR ir_object_layer;
    convert_object_layer_to_ir(model,
                               component_set_entity,
                               layer_entity,
                               ir_object_layer);

    CHECK(object_layer.objects.size() == ir_object_layer.objects.size());

    const auto& ir_point = ir_object_layer.objects.at(0);
    CHECK(ir_point.type == ObjectType::Point);
    CHECK(ir_point.id == 123);

    const auto& ir_ellipse = ir_object_layer.objects.at(1);
    CHECK(ir_ellipse.type == ObjectType::Ellipse);
    CHECK(ir_ellipse.id == 124);
  }

  TEST_CASE("convert_group_layer_to_ir")
  {
    auto model = sys::create_model(BackendAPI::Null);
    const auto component_set_entity = sys::create_component_set(model);

    const auto tile_layer_entity = LayerBuilder {model}
                                       .as_tile_layer({4, 8})
                                       .set_tile({2, 4}, 84)
                                       .set_tile({0, 2}, 7)
                                       .set_tile({3, 7}, 999)
                                       .build();

    const auto object_layer_entity = LayerBuilder {model}
                                         .as_object_layer()
                                         .with_object(1, ObjectType::Point)
                                         .with_object(2, ObjectType::Rect)
                                         .with_object(3, ObjectType::Ellipse)
                                         .build();

    const auto group_layer_entity = LayerBuilder {model}
                                        .as_group_layer()
                                        .with_layer(tile_layer_entity)
                                        .with_layer(object_layer_entity)
                                        .build();

    GroupLayerIR ir_group_layer;
    convert_group_layer_to_ir(model,
                              component_set_entity,
                              group_layer_entity,
                              ir_group_layer);

    CHECK(ir_group_layer.children.size() == 2);

    const auto& ir_tile_layer = ir_group_layer.children.at(0);
    CHECK(ir_tile_layer->type == LayerType::TileLayer);
    CHECK(ir_tile_layer->index == 0);

    const auto& ir_object_layer = ir_group_layer.children.at(1);
    CHECK(ir_object_layer->type == LayerType::ObjectLayer);
    CHECK(ir_object_layer->index == 1);
  }

  TEST_CASE("convert_layer_to_ir")
  {
    auto model = sys::create_model(BackendAPI::Null);
    const auto component_set_entity = sys::create_component_set(model);

    const auto layer_entity = LayerBuilder {model}
                                  .with_id(42)
                                  .with_name("Layer 1")
                                  .with_opacity(0.75f)
                                  .with_visibility(false)
                                  .with_property("threshold", 100)
                                  .with_property("is-cool", true)
                                  .as_tile_layer({10, 10})
                                  .set_tile({0, 0}, 10)
                                  .set_tile({5, 3}, 849)
                                  .set_tile({9, 9}, 781)
                                  .build();

    LayerIR ir_layer;
    convert_layer_to_ir(model, component_set_entity, layer_entity, 10, ir_layer);

    CHECK(ir_layer.type == LayerType::TileLayer);
    CHECK(ir_layer.index == 10);
    CHECK(ir_layer.id == 42);
    CHECK(ir_layer.name == "Layer 1");
    CHECK(ir_layer.opacity == 0.75f);
    CHECK(!ir_layer.visible);
    CHECK(ir_layer.context.components.empty());
    CHECK(ir_layer.context.properties.size() == 2);
    CHECK(ir_layer.context.properties.at("threshold") == 100);
    CHECK(ir_layer.context.properties.at("is-cool") == true);

    const auto& ir_tile_layer = std::get<TileLayerIR>(ir_layer.data);
    CHECK(ir_tile_layer.extent.rows == 10);
    CHECK(ir_tile_layer.extent.cols == 10);
    CHECK(ir_tile_layer.tiles.at(0).at(0) == 10);
    CHECK(ir_tile_layer.tiles.at(5).at(3) == 849);
    CHECK(ir_tile_layer.tiles.at(9).at(9) == 781);
  }
}
