#include "json_parser.hpp"

#include <string>  // string

#include <nlohmann/json.hpp>

#include "io/maps/json_utils.hpp"
#include "json_attributes.hpp"
#include "json_layers.hpp"
#include "json_tilesets.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _validate_map(const nlohmann::json& json) -> parse_error
{
  if (const auto iter = json.find("orientation");
      iter == json.end() || iter->get<std::string>() != "orthogonal") {
    return parse_error::unsupported_map_orientation;
  }

  if (const auto iter = json.find("infinite"); iter != json.end() && iter->get<bool>()) {
    return parse_error::unsupported_infinite_map;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_map(const std::filesystem::path& path, ir::map_data& mapData)
    -> parse_error
{
  const auto json = read_json(path);
  if (!json) {
    return parse_error::could_not_read_file;
  }

  if (const auto err = _validate_map(*json); err != parse_error::none) {
    return err;
  }

  if (const auto width = as_uint(*json, "width")) {
    mapData.col_count = *width;
  }
  else {
    return parse_error::no_map_width;
  }

  if (const auto height = as_uint(*json, "height")) {
    mapData.row_count = *height;
  }
  else {
    return parse_error::no_map_height;
  }

  if (const auto tw = as_int(*json, "tilewidth")) {
    mapData.tile_width = *tw;
  }
  else {
    return parse_error::no_map_tile_width;
  }

  if (const auto th = as_int(*json, "tileheight")) {
    mapData.tile_height = *th;
  }
  else {
    return parse_error::no_map_tile_height;
  }

  if (const auto id = as_int(*json, "nextlayerid")) {
    mapData.next_layer_id = *id;
  }
  else {
    return parse_error::no_map_next_layer_id;
  }

  if (const auto id = as_int(*json, "nextobjectid")) {
    mapData.next_object_id = *id;
  }
  else {
    return parse_error::no_map_next_object_id;
  }

  const auto dir = path.parent_path();

  if (const auto err = parse_tilesets(*json, mapData, dir); err != parse_error::none) {
    return err;
  }

  if (const auto err = parse_layers(*json, mapData); err != parse_error::none) {
    return err;
  }

  if (const auto err = parse_properties(*json, mapData.context);
      err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

}  // namespace

auto parse_json_map(const std::filesystem::path& path) -> parse_data
{
  parse_data result;
  result.set_path(path);

  const auto err = _parse_map(path, result.data());
  result.set_error(err);

  return result;
}

}  // namespace tactile::parsing