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

#include "model/layers/tile_layer_ops.hpp"

#include <doctest/doctest.h>

#include "helpers/layer_builder.hpp"
#include "model/model_factory.hpp"

using namespace tactile;

TEST_SUITE("TileLayerOps")
{
  TEST_CASE("flood_tiles")
  {
    auto registry = sys::create_model(BackendAPI::Null);
    const auto layer_entity = LayerBuilder {registry}.as_tile_layer({5, 5}).build();

    const TileMatrix initial_tiles = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 0},
    };

    auto& tile_layer = registry.get<TileLayer>(layer_entity);
    tile_layer.tiles = initial_tiles;

    const TileMatrix expected_tiles_after_flood_1 = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1},
        {0, 0, 0, 1, 2},
        {0, 1, 1, 1, 2},
        {1, 2, 2, 2, 2},
    };

    sys::flood_tiles(tile_layer, {4, 1}, 2);
    CHECK(tile_layer.tiles == expected_tiles_after_flood_1);

    const TileMatrix expected_tiles_after_flood_2 = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 3, 3},
        {0, 0, 0, 3, 2},
        {0, 3, 3, 3, 2},
        {1, 2, 2, 2, 2},
    };

    sys::flood_tiles(tile_layer, {3, 1}, 3);
    CHECK(tile_layer.tiles == expected_tiles_after_flood_2);

    const TileMatrix expected_tiles_after_flood_3 = {
        {4, 4, 4, 4, 4},
        {4, 4, 4, 3, 3},
        {4, 4, 4, 3, 2},
        {4, 3, 3, 3, 2},
        {1, 2, 2, 2, 2},
    };

    sys::flood_tiles(tile_layer, {0, 0}, 4);
    CHECK(tile_layer.tiles == expected_tiles_after_flood_3);
  }

  TEST_CASE("set_tile")
  {
    auto registry = sys::create_model(BackendAPI::Null);

    const auto layer_entity = LayerBuilder {registry}.as_tile_layer({4, 4}).build();
    auto& tile_layer = registry.get<TileLayer>(layer_entity);

    SUBCASE("Initial tiles should be empty")
    {
      const TileMatrix expected_tile = {
          {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 0, 0, 0},
      };

      CHECK(tile_layer.tiles == expected_tile);
    }

    SUBCASE("Try setting corner tiles")
    {
      CHECK(sys::set_tile(tile_layer, {0, 0}, 1).succeeded());
      CHECK(sys::set_tile(tile_layer, {0, 3}, 2).succeeded());
      CHECK(sys::set_tile(tile_layer, {3, 0}, 3).succeeded());
      CHECK(sys::set_tile(tile_layer, {3, 3}, 4).succeeded());

      const TileMatrix expected_tile = {
          {1, 0, 0, 2},
          {0, 0, 0, 0},
          {0, 0, 0, 0},
          {3, 0, 0, 4},
      };

      CHECK(tile_layer.tiles == expected_tile);
    }

    SUBCASE("Invalid positions should leave the layer unaffected")
    {
      CHECK(sys::set_tile(tile_layer, {-1, 0}, 999).failed());
      CHECK(sys::set_tile(tile_layer, {0, -1}, 999).failed());
      CHECK(sys::set_tile(tile_layer, {5, 0}, 999).failed());
      CHECK(sys::set_tile(tile_layer, {0, 5}, 999).failed());

      const TileMatrix expected_tiles = {
          {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 0, 0, 0},
          {0, 0, 0, 0},
      };

      CHECK(tile_layer.tiles == expected_tiles);
    }
  }

  TEST_CASE("tile_at")
  {
    auto registry = sys::create_model(BackendAPI::Null);

    const auto layer_entity = LayerBuilder {registry}.as_tile_layer({4, 4}).build();
    auto& tile_layer = registry.get<TileLayer>(layer_entity);

    tile_layer.tiles = TileMatrix {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16},
    };

    CHECK(!sys::tile_at(tile_layer, {-1, 0}).has_value());
    CHECK(!sys::tile_at(tile_layer, {0, -1}).has_value());
    CHECK(!sys::tile_at(tile_layer, {-1, -1}).has_value());
    CHECK(!sys::tile_at(tile_layer, {4, 0}).has_value());
    CHECK(!sys::tile_at(tile_layer, {0, 4}).has_value());
    CHECK(!sys::tile_at(tile_layer, {4, 4}).has_value());

    CHECK(sys::tile_at(tile_layer, {0, 0}) == 1);
    CHECK(sys::tile_at(tile_layer, {0, 1}) == 2);
    CHECK(sys::tile_at(tile_layer, {0, 2}) == 3);
    CHECK(sys::tile_at(tile_layer, {0, 3}) == 4);

    CHECK(sys::tile_at(tile_layer, {1, 0}) == 5);
    CHECK(sys::tile_at(tile_layer, {1, 1}) == 6);
    CHECK(sys::tile_at(tile_layer, {1, 2}) == 7);
    CHECK(sys::tile_at(tile_layer, {1, 3}) == 8);

    CHECK(sys::tile_at(tile_layer, {2, 0}) == 9);
    CHECK(sys::tile_at(tile_layer, {2, 1}) == 10);
    CHECK(sys::tile_at(tile_layer, {2, 2}) == 11);
    CHECK(sys::tile_at(tile_layer, {2, 3}) == 12);

    CHECK(sys::tile_at(tile_layer, {3, 0}) == 13);
    CHECK(sys::tile_at(tile_layer, {3, 1}) == 14);
    CHECK(sys::tile_at(tile_layer, {3, 2}) == 15);
    CHECK(sys::tile_at(tile_layer, {3, 3}) == 16);
  }

  TEST_CASE("is_valid_tile")
  {
    auto registry = sys::create_model(BackendAPI::Null);

    const auto layer_entity = LayerBuilder {registry}.as_tile_layer({4, 4}).build();
    const auto& tile_layer = registry.get<TileLayer>(layer_entity);

    CHECK(!sys::is_valid_tile(tile_layer, {-1, 0}));
    CHECK(!sys::is_valid_tile(tile_layer, {0, -1}));
    CHECK(!sys::is_valid_tile(tile_layer, {4, 3}));
    CHECK(!sys::is_valid_tile(tile_layer, {3, 4}));
    CHECK(sys::is_valid_tile(tile_layer, {0, 0}));
    CHECK(sys::is_valid_tile(tile_layer, {2, 1}));
    CHECK(sys::is_valid_tile(tile_layer, {3, 3}));
  }
}