#pragma once

#include <filesystem>  // path

namespace Tactile {

[[nodiscard]] auto GetPersistentFileDir() -> const std::filesystem::path&;

}  // namespace Tactile
