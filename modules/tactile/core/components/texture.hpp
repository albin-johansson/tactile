#pragma once

#include <filesystem>  // path

#include <tactile-base/tactile_std.hpp>

namespace Tactile {

struct Texture final
{
  uint id{};
  int width{};
  int height{};
  std::filesystem::path path;
};

}  // namespace Tactile