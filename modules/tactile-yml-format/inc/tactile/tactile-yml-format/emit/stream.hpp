// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/io/compression/compression_kind.hpp"
#include "tactile/foundation/meta/attribute.hpp"
#include "tactile/foundation/meta/attribute_type.hpp"
#include "tactile/foundation/misc/tile_encoding.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile {

auto operator<<(YAML::Emitter& emitter, AttributeType type) -> YAML::Emitter&;

auto operator<<(YAML::Emitter& emitter, const Attribute& value) -> YAML::Emitter&;

auto operator<<(YAML::Emitter& emitter, TileEncoding tile_encoding) -> YAML::Emitter&;

auto operator<<(YAML::Emitter& emitter, CompressionKind compression_kind)
    -> YAML::Emitter&;

}  // namespace tactile
