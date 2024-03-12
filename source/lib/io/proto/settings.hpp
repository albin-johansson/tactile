// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/settings.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/// Parses the persistent settings, or returns the default ones if there are none.
[[nodiscard]] auto load_settings_from_disk() -> Settings;

/// Saves settings to the persistent file directory.
void save_settings_to_disk(const Settings& settings);

}  // namespace tactile
