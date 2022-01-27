#pragma once

#include <filesystem>  // path

#include <tactile_def.hpp>

namespace tactile::comp {

struct texture final
{
  uint id{};
  int width{};
  int height{};
  std::filesystem::path path;
};

}  // namespace tactile::comp