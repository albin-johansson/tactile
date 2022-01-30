#include "parse_error.hpp"

#include <magic_enum.hpp>

#include "tactile_stdlib.hpp"
#include "throw.hpp"

namespace tactile::parsing {

auto to_cause(const parse_error error) -> std::string_view
{
  switch (error) {
    case parse_error::none:
      return "Nothing went wrong!"; /* This case isn't really meant to be used */

    case parse_error::unknown:
      return "An unknown error occurred!";

    case parse_error::could_not_read_file:
      return "Could not read a file!";

    case parse_error::map_does_not_exist:
      return "The map does not exist!";

    case parse_error::unsupported_map_extension:
      return "The map features an unsupported file extension!";

    case parse_error::unsupported_map_orientation:
      return "The map uses an unsupported tile orientation!";

    case parse_error::unsupported_infinite_map:
      return "Infinite are not supported!";

    case parse_error::no_map_width:
      return "The map features no width attribute!";

    case parse_error::no_map_height:
      return "The map features no height attribute!";

    case parse_error::no_map_tile_width:
      return "The map features no tile width attribute!";

    case parse_error::no_map_tile_height:
      return "The map features no tile height attribute!";

    case parse_error::no_map_next_layer_id:
      return "The map features no next layer identifier attribute!";

    case parse_error::no_map_next_object_id:
      return "The map features no next object identifier attribute!";

    case parse_error::no_map_tilesets:
      return "The map features no tilesets attribute!";

    case parse_error::no_map_layers:
      return "The map features no layers attribute!";

    case parse_error::unsupported_layer_type:
      return "A layer is of an unsupported type!";

    case parse_error::no_layer_id:
      return "A layer is missing an identifier!";

    case parse_error::no_layer_type:
      return "A layer is missing a type!";

    case parse_error::no_tile_layer_width:
      return "A tile layer is missing a width attribute!";

    case parse_error::no_tile_layer_height:
      return "A tile layer is missing a height attribute!";

    case parse_error::no_tile_layer_data:
      return "A tile layer has no tile data!";

    case parse_error::corrupt_tile_layer_data:
      return "A tile layer has corrupt tile data!";

    case parse_error::unsupported_tile_layer_encoding:
      return "A tile layer uses an unsupported tile encoding!";

    case parse_error::external_tileset_does_not_exist:
      return "A referenced external tileset does not exist!";

    case parse_error::could_not_read_external_tileset:
      return "Could not read an external tileset!";

    case parse_error::no_tileset_first_tile_id:
      return "A tileset has no associated first tile attribute!";

    case parse_error::no_tileset_tile_width:
      return "A tileset has no tile width attribute!";

    case parse_error::no_tileset_tile_height:
      return "A tileset has no tile height attribute!";

    case parse_error::no_tileset_name:
      return "A tileset has no name attribute!";

    case parse_error::no_tileset_image_path:
      return "A tileset has no image path attribute!";

    case parse_error::no_tileset_image_width:
      return "A tileset has no image width attribute!";

    case parse_error::no_tileset_image_height:
      return "A tileset has no image height attribute!";

    case parse_error::no_tileset_tile_count:
      return "A tileset has no tile count attribute!";

    case parse_error::no_tileset_column_count:
      return "A tileset has no column count attribute!";

    case parse_error::no_tileset_version:
      return "A tileset has no version attribute!";

    case parse_error::no_external_tileset_path:
      return "A referenced external tileset has no source path attribute!";

    case parse_error::tileset_image_does_not_exist:
      return "A referenced tileset image does not exist!";

    case parse_error::unsupported_tileset_version:
      return "A tileset is of an unsupported format version!";

    case parse_error::could_not_parse_property:
      return "A property value is invalid!";

    case parse_error::no_property_name:
      return "A property has no name!";

    case parse_error::no_property_type:
      return "A property has no type!";

    case parse_error::unsupported_property_type:
      return "A property uses an unsupported type!";

    case parse_error::no_component_def_name:
      return "A component definition has no name!";

    case parse_error::no_component_def_attribute_name:
      return "A component definition attribute has no name!";

    case parse_error::no_component_def_attribute_type:
      return "A component definition attribute has no type!";

    case parse_error::unsupported_component_def_attribute_type:
      return "A component definition attribute is of an unsupported type!";

    case parse_error::unsupported_component_def_attribute_value:
      return "A component definition attribute has an unsupported value!";

    case parse_error::no_component_type:
      return "A component has no type!";

    case parse_error::no_component_attribute_name:
      return "A component attribute has no name!";

    case parse_error::no_component_attribute_value:
      return "A component attribute has no value!";

    case parse_error::unsupported_component_attribute_value:
      return "A component attribute has an invalid value!";

    case parse_error::no_object_id:
      return "An object has no identifier attribute!";

    case parse_error::no_object_type:
      return "An object has no type!";

    case parse_error::unsupported_object_type:
      return "An object is of an unsupported type!";

    case parse_error::no_fancy_tile_id:
      return "A tileset tile definition is missing a tile identifier!";

    case parse_error::no_animation_frame_tile:
      return "A tileset tile definition has an animation frame with no tile identifier!";

    case parse_error::no_animation_frame_duration:
      return "A tileset tile definition has an animation frame with no duration!";

    default:
      throw_traced(tactile_error{"Invalid parse error!"});
  }
}

auto operator<<(std::ostream& stream, parse_error error) -> std::ostream&
{
  return stream << magic_enum::enum_name<parse_error>(error);
}

}  // namespace tactile::parsing