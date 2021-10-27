#pragma once

#include <filesystem>  // path

#include "../yaml_fwd.hpp"
#include "ir.hpp"

namespace Tactile::IO {

struct EmitterOptions;

void SaveLayers(YAML::Emitter& emitter,
                const Map& map,
                const std::filesystem::path& dir,
                const EmitterOptions& options);

}  // namespace Tactile::IO
