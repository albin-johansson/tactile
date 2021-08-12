#ifndef RUNE_TMX_PARSE_TMX_HPP
#define RUNE_TMX_PARSE_TMX_HPP

#include <cassert>     // assert
#include <filesystem>  // path
#include <string>      // string

#include "../../aliases/integers.hpp"
#include "../json_utils.hpp"
#include "parse_tileset.hpp"
#include "tmx_global_id.hpp"
#include "tmx_local_id.hpp"
#include "tmx_map.hpp"

namespace rune {

/// \addtogroup tmx
/// \{

/**
 * \brief Parses a Tiled JSON map file, and returns the contents.
 *
 * \details This function will not validate the specified JSON file. An invalid map file
 * will likely result in an exception being thrown due to parsing errors. However, there
 * are debug assertions that try to make it easier to detect errors and their cause,
 * compared to the generic exceptions from the JSON parser.
 *
 * \note The Tiled XML format is not supported.
 *
 * \todo Support all versions of the JSON format.
 *
 * \param path the file path to the Tiled JSON map file.
 *
 * \return the parsed Tiled map.
 */
[[nodiscard]] inline auto parse_tmx(const std::filesystem::path& path) -> tmx_map
{
  const auto json = read_json(path);
  assert(json.contains("type") && "Map file requires \"type\" element!");
  assert(json.at("type").get<std::string>() == "map");

  tmx_map map;

  json.at("width").get_to(map.width);
  json.at("height").get_to(map.height);
  json.at("nextlayerid").get_to(map.next_layer_id);
  json.at("nextobjectid").get_to(map.next_object_id);
  json.at("tilewidth").get_to(map.tile_width);
  json.at("tileheight").get_to(map.tile_height);
  json.at("infinite").get_to(map.infinite);
  json.at("orientation").get_to(map.orientation);
  json.at("renderorder").get_to(map.render_order);
  json.at("tiledversion").get_to(map.tiled_version);
  json.at("version").get_to(map.json_version);

  io::try_get_to(json, "backgroundcolor", map.background);
  io::try_get_to(json, "compressionlevel", map.compression_level);
  io::try_get_to(json, "hexsidelength", map.hex_side_length);
  io::try_get_to(json, "staggeraxis", map.stagger_axis);
  io::try_get_to(json, "staggerindex", map.stagger_index);

  io::try_get_to(json, "layers", map.layers);
  io::try_get_to(json, "properties", map.properties);

  if (const auto it = json.find("tilesets"); it != json.end())
  {
    const auto directory = path.parent_path();

    map.tilesets.reserve(it->size());
    for (const auto& [key, value] : it->items())
    {
      map.tilesets.push_back(tmx::parse_tileset(directory, value));
    }
  }

  return map;
}

/// \} End of group tmx

}  // namespace rune

#endif  // RUNE_TMX_PARSE_TMX_HPP
