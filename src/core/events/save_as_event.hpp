#pragma once

#include <filesystem>  // path

namespace Tactile {

struct SaveAsEvent final
{
  std::filesystem::path path;
};

}  // namespace Tactile
