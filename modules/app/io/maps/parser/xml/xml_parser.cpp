#include "xml_parser.hpp"

#include <filesystem>  // path, exists

#include <pugixml.hpp>

#include "io/maps/xml_utils.hpp"
#include "xml_attributes.hpp"
#include "xml_layers.hpp"
#include "xml_tilesets.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _validate_map(pugi::xml_node mapNode) -> parse_error
{
  if (string_attribute(mapNode, "orientation") != "orthogonal") {
    return parse_error::unsupported_map_orientation;
  }

  if (bool_attribute(mapNode, "infinite").value_or(false)) {
    return parse_error::unsupported_infinite_map;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_map(const std::filesystem::path& path, ir::map_data& data)
    -> parse_error
{
  if (!std::filesystem::exists(path)) {
    return parse_error::map_does_not_exist;
  }

  pugi::xml_document document;
  if (!document.load_file(path.c_str())) {
    return parse_error::could_not_read_file;
  }

  auto mapNode = document.child("map");

  if (const auto err = _validate_map(mapNode); err != parse_error::none) {
    return err;
  }

  if (const auto width = uint_attribute(mapNode, "width")) {
    data.col_count = *width;
  }
  else {
    return parse_error::no_map_width;
  }

  if (const auto height = uint_attribute(mapNode, "height")) {
    data.row_count = *height;
  }
  else {
    return parse_error::no_map_height;
  }

  if (const auto tw = int_attribute(mapNode, "tilewidth")) {
    data.tile_width = *tw;
  }
  else {
    return parse_error::no_map_tile_width;
  }

  if (const auto th = int_attribute(mapNode, "tileheight")) {
    data.tile_height = *th;
  }
  else {
    return parse_error::no_map_tile_height;
  }

  if (const auto id = int_attribute(mapNode, "nextlayerid")) {
    data.next_layer_id = *id;
  }
  else {
    return parse_error::no_map_next_layer_id;
  }

  if (const auto id = int_attribute(mapNode, "nextobjectid")) {
    data.next_object_id = *id;
  }
  else {
    return parse_error::no_map_next_object_id;
  }

  const auto dir = path.parent_path();

  for (auto tilesetNode : mapNode.children("tileset")) {
    auto& tilesetData = data.tilesets.emplace_back();
    if (const auto err = parse_tileset(tilesetNode, tilesetData, dir);
        err != parse_error::none) {
      return err;
    }
  }

  if (const auto err = parse_layers(mapNode, data); err != parse_error::none) {
    return err;
  }

  if (const auto err = parse_properties(mapNode, data.context);
      err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

}  // namespace

auto parse_xml_map(const std::filesystem::path& path) -> parse_data
{
  parse_data result;
  result.set_path(path);

  const auto err = _parse_map(path, result.data());
  result.set_error(err);

  return result;
}

}  // namespace tactile::parsing