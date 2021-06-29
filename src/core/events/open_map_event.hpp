#pragma once

#include <filesystem>  // path

namespace Tactile {

struct OpenMapEvent final
{
  std::filesystem::path path;
};

}  // namespace Tactile
