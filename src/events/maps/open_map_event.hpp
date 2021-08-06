#pragma once

#include <filesystem>  // path

namespace Tactile {

/// \addtogroup events
/// \{

struct OpenMapEvent final
{
  std::filesystem::path path;
};

/// \} End of group events

}  // namespace Tactile
