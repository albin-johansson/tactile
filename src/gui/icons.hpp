#pragma once

#include "aliases/ints.hpp"

namespace Tactile {

void LoadIcons();

void UnloadIcons();

[[nodiscard]] auto GetTactileIcon() noexcept -> uint;

}  // namespace Tactile
