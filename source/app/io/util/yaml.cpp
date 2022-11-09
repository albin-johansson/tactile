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

#include "yaml.hpp"

#include "core/util/filesystem.hpp"
#include "misc/panic.hpp"

namespace tactile::io {

auto operator<<(YAML::Emitter& emitter, const Attribute& value) -> YAML::Emitter&
{
  switch (value.type()) {
    case AttributeType::String:
      return emitter << value.as_string();

    case AttributeType::Int:
      emitter << value.as_int();
      break;

    case AttributeType::Float:
      emitter << value.as_float();
      break;

    case AttributeType::Bool:
      emitter << value.as_bool();
      break;

    case AttributeType::Path:
      emitter << convert_to_forward_slashes(value.as_path());
      break;

    case AttributeType::Color:
      emitter << value.as_color().as_rgba();
      break;

    case AttributeType::Object:
      emitter << value.as_object();
      break;

    default:
      throw TactileError {"Invalid attribute type!"};
  }

  return emitter;
}

auto operator<<(YAML::Emitter& emitter, const AttributeType type) -> YAML::Emitter&
{
  return emitter << stringify(type);
}

auto operator<<(YAML::Emitter& emitter, const TileEncoding encoding) -> YAML::Emitter&
{
  switch (encoding) {
    case TileEncoding::Plain:
      return emitter << "plain";

    case TileEncoding::Base64:
      return emitter << "base64";

    default:
      throw TactileError {"Did not recognize tile encoding!"};
  }
}

auto operator<<(YAML::Emitter& emitter, const TileCompression compression)
    -> YAML::Emitter&
{
  switch (compression) {
    case TileCompression::None:
      return emitter << "none";

    case TileCompression::Zlib:
      return emitter << "zlib";

    case TileCompression::Zstd:
      return emitter << "zstd";

    default:
      throw TactileError {"Did not recognize tile compression!"};
  }
}

}  // namespace tactile::io
