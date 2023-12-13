// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

TACTILE_YML_FORMAT_API void emit(YAML::Emitter& emitter, AttributeType type);

TACTILE_YML_FORMAT_API void emit(YAML::Emitter& emitter, const Attribute& value);

TACTILE_YML_FORMAT_API void emit_metadata(YAML::Emitter& emitter,
                                          const ir::Metadata& metadata);

}  // namespace tactile
