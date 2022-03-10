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

#include "parse_error.hpp"

#include <magic_enum.hpp>

#include "misc/throw.hpp"

namespace tactile::parsing {

auto to_cause(const ParseError error) -> std::string_view
{
  switch (error) {
    case ParseError::none:
      return "Nothing went wrong!"; /* This case isn't really meant to be used */

    case ParseError::unknown:
      return "An unknown error occurred!";

    case ParseError::could_not_read_file:
      return "Could not read a file!";

    case ParseError::map_does_not_exist:
      return "The map does not exist!";

    case ParseError::unsupported_map_extension:
      return "The map features an unsupported file extension!";

    case ParseError::unsupported_map_orientation:
      return "The map uses an unsupported tile orientation!";

    case ParseError::unsupported_infinite_map:
      return "Infinite are not supported!";

    case ParseError::no_map_width:
      return "The map features no width attribute!";

    case ParseError::no_map_height:
      return "The map features no height attribute!";

    case ParseError::no_map_tile_width:
      return "The map features no tile width attribute!";

    case ParseError::no_map_tile_height:
      return "The map features no tile height attribute!";

    case ParseError::no_map_next_layer_id:
      return "The map features no next layer identifier attribute!";

    case ParseError::no_map_next_object_id:
      return "The map features no next object identifier attribute!";

    case ParseError::unsupported_layer_type:
      return "A layer is of an unsupported type!";

    case ParseError::no_layer_id:
      return "A layer is missing an identifier!";

    case ParseError::no_layer_type:
      return "A layer is missing a type!";

    case ParseError::no_tile_layer_data:
      return "A tile layer has no tile data!";

    case ParseError::corrupt_tile_layer_data:
      return "A tile layer has corrupt tile data!";

    case ParseError::unsupported_tile_layer_encoding:
      return "A tile layer uses an unsupported tile encoding!";

    case ParseError::external_tileset_does_not_exist:
      return "A referenced external tileset does not exist!";

    case ParseError::unknown_external_tileset_error:
      return "Something went wrong when trying to read an external tileset!";

    case ParseError::no_tileset_first_tile_id:
      return "A tileset has no associated first tile attribute!";

    case ParseError::no_tileset_tile_width:
      return "A tileset has no tile width attribute!";

    case ParseError::no_tileset_tile_height:
      return "A tileset has no tile height attribute!";

    case ParseError::no_tileset_name:
      return "A tileset has no name attribute!";

    case ParseError::no_tileset_image_path:
      return "A tileset has no image path attribute!";

    case ParseError::no_tileset_image_width:
      return "A tileset has no image width attribute!";

    case ParseError::no_tileset_image_height:
      return "A tileset has no image height attribute!";

    case ParseError::no_tileset_tile_count:
      return "A tileset has no tile count attribute!";

    case ParseError::no_tileset_column_count:
      return "A tileset has no column count attribute!";

    case ParseError::no_tileset_version:
      return "A tileset has no version attribute!";

    case ParseError::no_external_tileset_path:
      return "An external tileset reference is missing a source path attribute!";

    case ParseError::tileset_image_does_not_exist:
      return "A referenced tileset image does not exist!";

    case ParseError::unsupported_tileset_version:
      return "A tileset is of an unsupported format version!";

    case ParseError::corrupt_property_value:
      return "A property value is invalid!";

    case ParseError::no_property_name:
      return "A property has no name!";

    case ParseError::no_property_type:
      return "A property has no type!";

    case ParseError::unsupported_property_type:
      return "A property uses an unsupported type!";

    case ParseError::no_component_def_name:
      return "A component definition has no name!";

    case ParseError::no_component_def_attribute_name:
      return "A component definition attribute has no name!";

    case ParseError::no_component_def_attribute_type:
      return "A component definition attribute has no type!";

    case ParseError::unsupported_component_def_attribute_type:
      return "A component definition attribute is of an unsupported type!";

    case ParseError::corrupt_component_def_attribute_value:
      return "A component definition attribute has an unsupported value!";

    case ParseError::no_component_type:
      return "A component has no type!";

    case ParseError::no_component_attribute_name:
      return "A component attribute has no name!";

    case ParseError::no_component_attribute_value:
      return "A component attribute has no value!";

    case ParseError::corrupt_component_attribute_value:
      return "A component attribute has an invalid value!";

    case ParseError::no_object_id:
      return "An object has no identifier attribute!";

    case ParseError::no_object_type:
      return "An object has no type!";

    case ParseError::unsupported_object_type:
      return "An object is of an unsupported type!";

    case ParseError::no_fancy_tile_id:
      return "A tileset tile definition is missing a tile identifier!";

    case ParseError::no_animation_frame_tile:
      return "A tileset tile definition has an animation frame with no tile identifier!";

    case ParseError::no_animation_frame_duration:
      return "A tileset tile definition has an animation frame with no duration!";

    default:
      throw_traced(tactile_error{"Invalid parse error!"});
  }
}

auto operator<<(std::ostream& stream, const ParseError error) -> std::ostream&
{
  return stream << magic_enum::enum_name<ParseError>(error);
}

}  // namespace tactile::parsing