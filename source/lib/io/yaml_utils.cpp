/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "yaml_utils.hpp"

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/util/filesystem.hpp"
#include "io/save_formats.hpp"
#include "ui/stream_utils.hpp"

namespace tactile {

auto save_yaml_to_file(const YAML::Emitter& emitter, const Path& path) -> Result
{
  auto stream = open_output_stream(path, FileType::Text);
  if (stream.has_value()) {
    *stream << emitter.c_str();
    return success;
  }
  else {
    spdlog::error("[YAML] Could not write to YAML file at {}", path);
    return failure;
  }
}

auto operator<<(YAML::Emitter& emitter, const Attribute& value) -> YAML::Emitter&
{
  switch (value.get_type()) {
    case AttributeType::String:
      return emitter << value.as_string();

    case AttributeType::Int:
      emitter << value.as_int();
      break;

    case AttributeType::Int2:
      emitter << serialize_to_save_format(value.as_int2());
      break;

    case AttributeType::Int3:
      emitter << serialize_to_save_format(value.as_int3());
      break;

    case AttributeType::Int4:
      emitter << serialize_to_save_format(value.as_int4());
      break;

    case AttributeType::Float:
      emitter << value.as_float();
      break;

    case AttributeType::Float2:
      emitter << serialize_to_save_format(value.as_float2());
      break;

    case AttributeType::Float3:
      emitter << serialize_to_save_format(value.as_float3());
      break;

    case AttributeType::Float4:
      emitter << serialize_to_save_format(value.as_float4());
      break;

    case AttributeType::Bool:
      emitter << value.as_bool();
      break;

    case AttributeType::Path:
      emitter << to_forward_slashes_path(value.as_path());
      break;

    case AttributeType::Color:
      emitter << value.as_color().as_rgba();
      break;

    case AttributeType::Object:
      emitter << value.as_object();
      break;

    default:
      throw TactileError {"Invalid attribute type"};
  }

  return emitter;
}

auto operator<<(YAML::Emitter& emitter, const AttributeType type) -> YAML::Emitter&
{
  return emitter << serialize_to_save_format(type).data();
}

auto operator<<(YAML::Emitter& emitter, const TileEncoding encoding) -> YAML::Emitter&
{
  switch (encoding) {
    case TileEncoding::Plain:
      return emitter << "plain";

    case TileEncoding::Base64:
      return emitter << "base64";

    default:
      throw TactileError {"Did not recognize tile encoding"};
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
      throw TactileError {"Did not recognize tile compression"};
  }
}

}  // namespace tactile
