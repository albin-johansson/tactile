#pragma once

#include <filesystem>  // path

namespace tactile {

[[nodiscard]] auto GetPersistentFilePath() -> const std::filesystem::path&;

}  // namespace tactile
