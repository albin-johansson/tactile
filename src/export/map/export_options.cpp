#include "export_options.hpp"

#include "preferences.hpp"

namespace tactile {

auto MakeExportOptions() -> ExportOptions
{
  ExportOptions options{};

  options.generateDefaults =
      prefs::GenerateDefaults().value_or(prefs::generate_defaults_def);
  options.embedTilesets =
      prefs::EmbedTilesets().value_or(prefs::embed_tilesets_def);

  return options;
}

}  // namespace tactile
