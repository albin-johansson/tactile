// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tmj-format/emit/tmj_tile_emitter.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(TmjTileEmitter, EmitEmptyTileDefinition)
{
  const ir::Tile tile {
    .meta = {},
    .index = 182,
    .objects = {},
    .animation = {},
  };

  const auto tile_json = tmj::emit_tile_definition(tile);

  EXPECT_EQ(tile_json["id"], tile.index.value);
  EXPECT_FALSE(tile_json.contains("properties"));
  EXPECT_FALSE(tile_json.contains("animation"));
  EXPECT_FALSE(tile_json.contains("objectgroup"));
}

TEST(TmjTileEmitter, EmitComplexTileDefinition)
{
  const ir::Tile tile {
    .meta =
        {
          .name = "T42",
          .properties =
              {
                ir::NamedAttribute {.name = "P1", .value = 100},
              },
          .components = {},
        },
    .index = 49,
    .objects =
        {
          ir::Object {},
          ir::Object {},
          ir::Object {},
        },
    .animation =
        {
          ir::AnimationFrame {.tile_index = 42, .duration_ms = 500},
          ir::AnimationFrame {.tile_index = 50, .duration_ms = 600},
          ir::AnimationFrame {.tile_index = 93, .duration_ms = 300},
          ir::AnimationFrame {.tile_index = 6, .duration_ms = 123},
        },
  };

  const auto tile_json = tmj::emit_tile_definition(tile);

  EXPECT_EQ(tile_json["id"], tile.index.value);
  ASSERT_TRUE(tile_json.contains("properties"));
  ASSERT_TRUE(tile_json.contains("animation"));
  ASSERT_TRUE(tile_json.contains("objectgroup"));

  const auto& properties_json = tile_json.at("properties");
  EXPECT_EQ(properties_json.size(), tile.meta.properties.size());
  EXPECT_EQ(properties_json.at(0).at("name"), "P1");
  EXPECT_EQ(properties_json.at(0).at("value"), 100);

  const auto& animation_json = tile_json.at("animation");
  EXPECT_EQ(animation_json.size(), tile.animation.size());
  EXPECT_EQ(animation_json.at(0).at("tileid"), tile.animation.at(0).tile_index.value);
  EXPECT_EQ(animation_json.at(0).at("duration"), tile.animation.at(0).duration_ms);
  EXPECT_EQ(animation_json.at(1).at("tileid"), tile.animation.at(1).tile_index.value);
  EXPECT_EQ(animation_json.at(1).at("duration"), tile.animation.at(1).duration_ms);
  EXPECT_EQ(animation_json.at(2).at("tileid"), tile.animation.at(2).tile_index.value);
  EXPECT_EQ(animation_json.at(2).at("duration"), tile.animation.at(2).duration_ms);
  EXPECT_EQ(animation_json.at(3).at("tileid"), tile.animation.at(3).tile_index.value);
  EXPECT_EQ(animation_json.at(3).at("duration"), tile.animation.at(3).duration_ms);

  EXPECT_EQ(tile_json.at("objectgroup").at("objects").size(), tile.objects.size());
}