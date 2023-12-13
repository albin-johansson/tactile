// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/emit/stream.hpp"

#include "tactile/tactile-yml-format/emit/meta_emitter.hpp"
#include "tactile/tactile-yml-format/emit/tile_format_emitter.hpp"

namespace tactile {

auto operator<<(YAML::Emitter& emitter, const AttributeType type) -> YAML::Emitter&
{
  yml_format::emit(emitter, type);
  return emitter;
}

auto operator<<(YAML::Emitter& emitter, const Attribute& value) -> YAML::Emitter&
{
  yml_format::emit(emitter, value);
  return emitter;
}

auto operator<<(YAML::Emitter& emitter, const TileEncoding tile_encoding)
    -> YAML::Emitter&
{
  yml_format::emit(emitter, tile_encoding);
  return emitter;
}

auto operator<<(YAML::Emitter& emitter, const CompressionKind compression_kind)
    -> YAML::Emitter&
{
  yml_format::emit(emitter, compression_kind);
  return emitter;
}

}  // namespace tactile
