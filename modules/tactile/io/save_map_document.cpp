#include "save_map_document.hpp"

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

void SaveMapDocument(const Document& document)
{
  assert(!document.path.empty());
  TACTILE_PROFILE_START

  const auto path = std::filesystem::absolute(document.path);
  CENTURION_LOG_INFO("Saving map document to \"%s\"", path.string().c_str());

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
  else {
    CENTURION_LOG_ERROR("Failed to save map document due to invalid extension: %s",
                        extension.string().c_str());
  }

  TACTILE_PROFILE_END("IO::SaveMapDocument()")
}

}  // namespace Tactile::IO
