#pragma once

#include <filesystem>  // path

namespace Tactile {

/// \addtogroup events
/// \{

struct SaveEvent final {};

struct OpenSaveAsDialogEvent final {};

struct SaveAsEvent final {
  std::filesystem::path path;
};

/// \} End of group events

}  // namespace Tactile
