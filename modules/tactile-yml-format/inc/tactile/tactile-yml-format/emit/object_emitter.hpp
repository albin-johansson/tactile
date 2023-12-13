// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

TACTILE_YML_FORMAT_API void emit_object(YAML::Emitter& emitter, const ir::Object& object);

TACTILE_YML_FORMAT_API void emit_object_seq(YAML::Emitter& emitter,
                                            const Vector<ir::Object>& objects);

}  // namespace tactile::yml_format
