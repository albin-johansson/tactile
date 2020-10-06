#pragma once

#include "preferences.hpp"

namespace tactile::service {

struct export_options final
{
  bool generateDefaults;
  bool embedTilesets;
};

[[nodiscard]] inline auto make_export_options() -> export_options
{
  export_options options{};
  options.generateDefaults = prefs::saves::generate_defaults().value_or(false);
  options.embedTilesets = prefs::saves::embed_tilesets().value_or(true);
  return options;
}

}  // namespace tactile::service
