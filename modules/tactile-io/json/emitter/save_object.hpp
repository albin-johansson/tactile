#pragma once

#include <filesystem>  // path

#include "../json_common.hpp"
#include "ir.hpp"

namespace Tactile::IO {

[[nodiscard]] auto SaveObject(const Object& object, const std::filesystem::path& dir)
    -> JSON;

}  // namespace Tactile::IO