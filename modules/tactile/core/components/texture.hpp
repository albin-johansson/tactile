#pragma once

#include <filesystem>  // path

#include <tactile_def.hpp>

namespace Tactile {

struct Texture final {
  uint id{};
  int width{};
  int height{};
  std::filesystem::path path;
};

}  // namespace Tactile