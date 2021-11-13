#include "save_document.hpp"

#include <cassert>     // assert
#include <filesystem>  // absolute

#include <tactile_io.hpp>

#include <centurion.hpp>  // CENTURION_LOG_{}

#include "convert_document_to_ir.hpp"
#include "core/utils/profile.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

[[nodiscard]] auto GetEmitterOptions() -> IO::EmitterOptions
{
  IO::EmitterOptions options = 0;

  if (Prefs::GetEmbedTilesets()) {
    options |= IO::EmitterOption_EmbedTilesets;
  }

  if (Prefs::GetFoldTileData()) {
    options |= IO::EmitterOption_FoldTileData;
  }

  if (Prefs::GetIndentOutput()) {
    options |= IO::EmitterOption_IndentOutput;
  }

  return options;
}

}  // namespace

void SaveDocument(const Document& document)
{
  assert(!document.path.empty());
  TACTILE_PROFILE_START

  const auto path = std::filesystem::absolute(document.path);
  cen::log::info("Trying to save map to %S", path.c_str());

  const auto data = ConvertDocumentToIR(document);
  if (IO::EmitMap(*data, GetEmitterOptions())) {
    TACTILE_PROFILE_END("Emitted document")
  }
  else {
    cen::log::warn("Something went wrong when emitting the map!");
  }
}

}  // namespace Tactile
