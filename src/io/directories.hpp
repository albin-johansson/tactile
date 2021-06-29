#pragma once

#include <filesystem>  // path

namespace tactile {

[[nodiscard]] auto GetPersistentFileDir() -> const std::filesystem::path&;

}  // namespace tactile
