#include "yaml_parser.hpp"

#include <filesystem>  // exists

#include <yaml-cpp/yaml.h>

#include "yaml_attributes.hpp"
#include "yaml_layers.hpp"
#include "yaml_tilesets.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_map(const std::filesystem::path& path, map_data& data)
    -> parse_error
{
  if (!std::filesystem::exists(path)) {
    return parse_error::map_does_not_exist;
  }

  const auto node = YAML::LoadFile(path.string());
  if (!node) {
    return parse_error::could_not_read_file;
  }

  if (auto rows = node["row-count"]) {
    data.row_count = rows.as<usize>();
  }
  else {
    return parse_error::no_map_height;
  }

  if (auto cols = node["column-count"]) {
    data.col_count = cols.as<usize>();
  }
  else {
    return parse_error::no_map_width;
  }

  if (auto tw = node["tile-width"]) {
    data.tile_width = tw.as<int32>();
  }
  else {
    return parse_error::no_map_tile_width;
  }

  if (auto th = node["tile-height"]) {
    data.tile_height = th.as<int32>();
  }
  else {
    return parse_error::no_map_tile_height;
  }

  if (auto id = node["next-layer-id"]) {
    data.next_layer_id = id.as<layer_id>();
  }
  else {
    return parse_error::no_map_next_layer_id;
  }

  if (auto id = node["next-object-id"]) {
    data.next_object_id = id.as<object_id>();
  }
  else {
    return parse_error::no_map_next_object_id;
  }

  const auto dir = path.parent_path();

  if (const auto err = parse_component_definitions(node, data);
      err != parse_error::none) {
    return err;
  }

  if (auto seq = node["tilesets"]) {
    if (const auto err = parse_tilesets(seq, data, dir); err != parse_error::none) {
      return err;
    }
  }

  if (auto seq = node["layers"]) {
    if (const auto err = parse_layers(seq, data); err != parse_error::none) {
      return err;
    }
  }

  if (const auto err = parse_properties(node, data.context); err != parse_error::none) {
    return err;
  }

  if (const auto err = parse_components(node, data, data.context);
      err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

}  // namespace

auto parse_yaml_map(const std::filesystem::path& path) -> parse_data
{
  parse_data result;
  result.set_path(path);

  const auto error = _parse_map(path, result.data());
  result.set_error(error);

  return result;
}

}  // namespace tactile::parsing