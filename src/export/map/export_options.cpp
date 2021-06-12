#include "export_options.hpp"

#include "preferences.hpp"

namespace tactile {

auto MakeExportOptions() -> ExportOptions
{
  ExportOptions options{};

  options.generateDefaults =
      prefs::GenerateDefaults().ValueOr(prefs::generate_defaults_def);
  options.embedTilesets =
      prefs::EmbedTilesets().ValueOr(prefs::embed_tilesets_def);

  return options;
}

}  // namespace tactile
