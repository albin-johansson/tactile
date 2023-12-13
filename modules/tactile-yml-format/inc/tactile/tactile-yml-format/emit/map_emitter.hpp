// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

TACTILE_YML_FORMAT_API void emit_map(YAML::Emitter& emitter,
                                     const ir::Map& map,
                                     const SaveFormatWriteOptions& options);

}  // namespace tactile::yml_format
