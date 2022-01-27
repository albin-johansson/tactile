#include "save_document.hpp"

#include <filesystem>  // absolute

#include <tactile_io.hpp>

#include "assert.hpp"
#include "convert_document_to_ir.hpp"
#include "io/preferences.hpp"
#include "profile.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto GetEmitterOptions() -> IO::EmitterOptions
{
  IO::EmitterOptions options = 0;

  if (prefs::GetEmbedTilesets()) {
    options |= IO::EmitterOption_EmbedTilesets;
  }

  if (prefs::GetFoldTileData()) {
    options |= IO::EmitterOption_FoldTileData;
  }

  if (prefs::GetIndentOutput()) {
    options |= IO::EmitterOption_IndentOutput;
  }

  return options;
}

}  // namespace

void SaveDocument(const Document& document)
{
  TACTILE_ASSERT(!document.path.empty());
  TACTILE_PROFILE_START

  const auto path = std::filesystem::absolute(document.path);
  LogInfo("Trying to save map to {}", path);

  const auto data = ConvertDocumentToIR(document);
  if (IO::EmitMap(*data, GetEmitterOptions())) {
    TACTILE_PROFILE_END("Emitted document")
  }
  else {
    log_warning("Failed to emit map!");
  }
}

}  // namespace tactile
