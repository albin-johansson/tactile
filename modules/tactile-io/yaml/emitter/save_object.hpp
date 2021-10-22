#pragma once

#include <filesystem>  // path

#include "../yaml_fwd.hpp"
#include "parse_ir.hpp"

namespace Tactile::IO {

void SaveObject(YAML::Emitter& emitter,
                const Object& object,
                const std::filesystem::path& dir);

}  // namespace Tactile::IO
