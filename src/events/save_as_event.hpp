#pragma once

#include <filesystem>  // path

namespace Tactile {

/// \addtogroup events
/// \{

struct SaveAsEvent final
{
  std::filesystem::path path;
};

/// \} End of group events

}  // namespace Tactile
