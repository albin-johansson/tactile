#pragma once

#include <yaml-cpp/yaml.h>

#include "core/attribute_value.hpp"
#include "tactile.hpp"

namespace tactile {

auto operator<<(YAML::Emitter& emitter, const attribute_value& value) -> YAML::Emitter&;

auto operator<<(YAML::Emitter& emitter, attribute_type type) -> YAML::Emitter&;

}  // namespace tactile
