// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "yaml.hpp"

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "common/util/filesystem.hpp"
#include "io/stream.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/io/save/vec_serialization.hpp"

namespace tactile {

auto save_yaml_to_file(const YAML::Emitter& emitter, const Path& path) -> Result<void>
{
  auto stream = open_output_stream(path, FileType::Text);
  if (stream.has_value()) {
    *stream << emitter.c_str();
    return kOK;
  }
  else {
    spdlog::error("Could not open YAML file for writing: ", path);
    return unexpected(make_error(GenericError::kInvalidFile));
  }
}

auto operator<<(YAML::Emitter& emitter, const Attribute& value) -> YAML::Emitter&
{
  switch (value.get_type()) {
    case AttributeType::kStr: return emitter << value.as_string();

    case AttributeType::kInt: emitter << value.as_int(); break;

    case AttributeType::kInt2: emitter << serialize(value.as_int2()); break;

    case AttributeType::kInt3: emitter << serialize(value.as_int3()); break;

    case AttributeType::kInt4: emitter << serialize(value.as_int4()); break;

    case AttributeType::kFloat: emitter << value.as_float(); break;

    case AttributeType::kFloat2: emitter << serialize(value.as_float2()); break;

    case AttributeType::kFloat3: emitter << serialize(value.as_float3()); break;

    case AttributeType::kFloat4: emitter << serialize(value.as_float4()); break;

    case AttributeType::kBool: emitter << value.as_bool(); break;

    case AttributeType::kPath: emitter << use_forward_slashes(value.as_path()); break;

    case AttributeType::kColor: emitter << value.as_color().to_string_rgba(); break;

    case AttributeType::kObject: emitter << value.as_object(); break;

    default: throw Exception {"Invalid attribute type"};
  }

  return emitter;
}

auto operator<<(YAML::Emitter& emitter, const AttributeType type) -> YAML::Emitter&
{
  return emitter << serialize(type).data();
}

auto operator<<(YAML::Emitter& emitter, const TileEncoding encoding) -> YAML::Emitter&
{
  switch (encoding) {
    case TileEncoding::Plain: return emitter << "plain";

    case TileEncoding::Base64: return emitter << "base64";

    default: throw Exception {"Did not recognize tile encoding!"};
  }
}

auto operator<<(YAML::Emitter& emitter, const TileCompression compression)
    -> YAML::Emitter&
{
  switch (compression) {
    case TileCompression::None: return emitter << "none";

    case TileCompression::Zlib: return emitter << "zlib";

    case TileCompression::Zstd: return emitter << "zstd";

    default: throw Exception {"Did not recognize tile compression!"};
  }
}

}  // namespace tactile
