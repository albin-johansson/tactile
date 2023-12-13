// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

TACTILE_YML_FORMAT_API void emit_component(YAML::Emitter& emitter,
                                           const ir::Component& component);

TACTILE_YML_FORMAT_API void emit_component_seq(YAML::Emitter& emitter,
                                               const Vector<ir::Component>& components);

}  // namespace tactile::yml_format
