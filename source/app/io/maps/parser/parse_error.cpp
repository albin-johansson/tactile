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

#include "misc/panic.hpp"

namespace tactile::parsing {

auto to_cause(const ParseError error) -> std::string_view
{
  switch (error) {
    case ParseError::None:
      return "Nothing went wrong!"; /* This case isn't really meant to be used */

    case ParseError::Unknown:
      return "An unknown error occurred!";

    case ParseError::CouldNotReadFile:
      return "Could not read a file!";

    case ParseError::MapDoesNotExist:
      return "The map does not exist!";

    case ParseError::UnsupportedMapExtension:
      return "The map features an unsupported file extension!";

    case ParseError::UnsupportedMapOrientation:
      return "The map uses an unsupported tile orientation!";

    case ParseError::UnsupportedInfiniteMap:
      return "Infinite are not supported!";

    case ParseError::NoMapWidth:
      return "The map features no width attribute!";

    case ParseError::NoMapHeight:
      return "The map features no height attribute!";

    case ParseError::NoMapTileWidth:
      return "The map features no tile width attribute!";

    case ParseError::NoMapTileHeight:
      return "The map features no tile height attribute!";

    case ParseError::NoMapNextLayerId:
      return "The map features no next layer identifier attribute!";

    case ParseError::NoMapNextObjectId:
      return "The map features no next object identifier attribute!";

    case ParseError::UnsupportedLayerType:
      return "A layer is of an unsupported type!";

    case ParseError::NoLayerId:
      return "A layer is missing an identifier!";

    case ParseError::NoLayerType:
      return "A layer is missing a type!";

    case ParseError::NoTileLayerData:
      return "A tile layer has no tile data!";

    case ParseError::CorruptTileLayerData:
      return "A tile layer has corrupt tile data!";

    case ParseError::UnsupportedTileLayerEncoding:
      return "A tile layer uses an unsupported tile encoding!";

    case ParseError::ExternalTilesetDoesNotExist:
      return "A referenced external tileset does not exist!";

    case ParseError::UnknownExternalTilesetError:
      return "Something went wrong when trying to read an external tileset!";

    case ParseError::NoTilesetFirstTileId:
      return "A tileset has no associated first tile attribute!";

    case ParseError::NoTilesetTileWidth:
      return "A tileset has no tile width attribute!";

    case ParseError::NoTilesetTileHeight:
      return "A tileset has no tile height attribute!";

    case ParseError::NoTilesetName:
      return "A tileset has no name attribute!";

    case ParseError::NoTilesetImagePath:
      return "A tileset has no image path attribute!";

    case ParseError::NoTilesetImageWidth:
      return "A tileset has no image width attribute!";

    case ParseError::NoTilesetImageHeight:
      return "A tileset has no image height attribute!";

    case ParseError::NoTilesetTileCount:
      return "A tileset has no tile count attribute!";

    case ParseError::NoTilesetColumnCount:
      return "A tileset has no column count attribute!";

    case ParseError::NoTilesetVersion:
      return "A tileset has no version attribute!";

    case ParseError::NoExternalTilesetPath:
      return "An external tileset reference is missing a source path attribute!";

    case ParseError::TilesetImageDoesNotExist:
      return "A referenced tileset image does not exist!";

    case ParseError::UnsupportedTilesetVersion:
      return "A tileset is of an unsupported format version!";

    case ParseError::CorruptPropertyValue:
      return "A property value is invalid!";

    case ParseError::NoPropertyName:
      return "A property has no name!";

    case ParseError::NoPropertyType:
      return "A property has no type!";

    case ParseError::UnsupportedPropertyType:
      return "A property uses an unsupported type!";

    case ParseError::NoComponentDefName:
      return "A component definition has no name!";

    case ParseError::NoComponentDefAttributeName:
      return "A component definition attribute has no name!";

    case ParseError::NoComponentDefAttributeType:
      return "A component definition attribute has no type!";

    case ParseError::UnsupportedComponentDefAttributeType:
      return "A component definition attribute is of an unsupported type!";

    case ParseError::CorruptComponentDefAttributeValue:
      return "A component definition attribute has an unsupported value!";

    case ParseError::NoComponentType:
      return "A component has no type!";

    case ParseError::NoComponentAttributeName:
      return "A component attribute has no name!";

    case ParseError::NoComponentAttributeValue:
      return "A component attribute has no value!";

    case ParseError::CorruptComponentAttributeValue:
      return "A component attribute has an invalid value!";

    case ParseError::NoObjectId:
      return "An object has no identifier attribute!";

    case ParseError::NoObjectType:
      return "An object has no type!";

    case ParseError::UnsupportedObjectType:
      return "An object is of an unsupported type!";

    case ParseError::NoFancyTileId:
      return "A tileset tile definition is missing a tile identifier!";

    case ParseError::NoAnimationFrameTile:
      return "A tileset tile definition has an animation frame with no tile identifier!";

    case ParseError::NoAnimationFrameDuration:
      return "A tileset tile definition has an animation frame with no duration!";

    default:
      panic("Invalid parse error!");
  }
}

auto operator<<(std::ostream& stream, const ParseError error) -> std::ostream&
{
  return stream << magic_enum::enum_name<ParseError>(error);
}

}  // namespace tactile::parsing