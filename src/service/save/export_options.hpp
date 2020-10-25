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
  using namespace prefs::saves;

  export_options options{};
  options.generateDefaults =
      generate_defaults().value_or(generate_defaults_def());
  options.embedTilesets = embed_tilesets().value_or(embed_tilesets_def());
  return options;
}

}  // namespace tactile::service
