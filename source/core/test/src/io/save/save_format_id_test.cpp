// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/save/save_format_id.hpp"

#include <gtest/gtest.h>

namespace tactile {

/** \trace tactile::to_string [SaveFormatId] */
TEST(SaveFormatId, ToString)
{
  EXPECT_EQ(to_string(SaveFormatId::kTactileYaml), "Tactile YAML");
  EXPECT_EQ(to_string(SaveFormatId::kTiledJson), "Tiled JSON");
  EXPECT_EQ(to_string(SaveFormatId::kTiledXml), "Tiled XML");
  EXPECT_EQ(to_string(SaveFormatId::kGodotScene), "Godot scene");
}

/** \trace tactile::get_file_extension */
TEST(SaveFormatId, GetFileExtension)
{
  EXPECT_EQ(get_file_extension(SaveFormatId::kTactileYaml), ".yml");
  EXPECT_EQ(get_file_extension(SaveFormatId::kTiledJson), ".tmj");
  EXPECT_EQ(get_file_extension(SaveFormatId::kTiledXml), ".tmx");
  EXPECT_EQ(get_file_extension(SaveFormatId::kGodotScene), ".escn");
}

/** \trace tactile::has_valid_tactile_yaml_extension */
TEST(SaveFormatId, HasValidTactileYamlExtension)
{
  EXPECT_TRUE(has_valid_tactile_yaml_extension("foo.yml"));
  EXPECT_TRUE(has_valid_tactile_yaml_extension("foo.yaml"));

  EXPECT_FALSE(has_valid_tactile_yaml_extension("foo.bar"));
  EXPECT_FALSE(has_valid_tactile_yaml_extension("foo.json"));
}

/** \trace tactile::has_valid_tiled_json_extension */
TEST(SaveFormatId, HasValidTiledJsonExtension)
{
  EXPECT_TRUE(has_valid_tiled_json_extension("foo.json"));
  EXPECT_TRUE(has_valid_tiled_json_extension("foo.tmj"));

  EXPECT_FALSE(has_valid_tiled_json_extension("foo.jsn"));
  EXPECT_FALSE(has_valid_tiled_json_extension("foo.tmx"));
}

/** \trace tactile::has_valid_tiled_xml_extension */
TEST(SaveFormatId, HasValidTiledXmlExtension)
{
  EXPECT_TRUE(has_valid_tiled_xml_extension("foo.xml"));
  EXPECT_TRUE(has_valid_tiled_xml_extension("foo.tmx"));

  EXPECT_FALSE(has_valid_tiled_xml_extension("foo.xm"));
  EXPECT_FALSE(has_valid_tiled_xml_extension("foo.tmj"));
}

}  // namespace tactile
