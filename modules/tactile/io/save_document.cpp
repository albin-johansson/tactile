#include "save_document.hpp"

#include <cassert>     // assert
#include <filesystem>  // absolute

#include <tactile-io/emitter.hpp>

#include <centurion.hpp>  // CENTURION_LOG_{}

#include "convert_document_to_ir.hpp"
#include "core/utils/profile.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

[[nodiscard]] auto GetEmitterOptions() -> IO::EmitterOptions
{
  return {.embed_tilesets = Prefs::GetEmbedTilesets(),
          .human_readable_output = Prefs::GetHumanReadableOutput()};
}

}  // namespace

void SaveDocument(const Document& document)
{
  assert(!document.path.empty());
  TACTILE_PROFILE_START

  const auto path = std::filesystem::absolute(document.path);
  cen::log::info("Saving map to %s", path.string().c_str());

  const auto extension = path.extension();
  if (extension == ".json") {
    const auto data = ConvertDocumentToIR(document);
    IO::EmitJsonMap(*data, GetEmitterOptions());
  }
  else if (extension == ".tmx" || extension == ".xml") {
    const auto data = ConvertDocumentToIR(document);
    IO::EmitXmlMap(*data, GetEmitterOptions());
  }
  else if (extension == ".yml" || extension == ".yaml") {
    const auto data = ConvertDocumentToIR(document);
    IO::EmitYamlMap(*data, GetEmitterOptions());
  }

  TACTILE_PROFILE_END("Emitted document")
}

}  // namespace Tactile
