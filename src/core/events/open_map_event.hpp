#pragma once

#include <filesystem>  // path

namespace tactile {

struct OpenMapEvent final
{
  std::filesystem::path path;
};

}  // namespace tactile
