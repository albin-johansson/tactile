#include "save_map_document.hpp"

#include <cassert>     // assert
#include <filesystem>  // absolute

#include <centurion.hpp>  // CENTURION_LOG_{}

#include <tactile-io/emitter.hpp>

#include "convert_document_to_ir.hpp"
#include "io/preferences.hpp"
#include "tactile/core/utils/profile.hpp"

namespace Tactile::IO {
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
    EmitJsonMap(data, GetEmitterOptions());
  }
  else if (extension == ".tmx" || extension == ".xml") {
    const auto data = ConvertDocumentToIR(document);
    EmitXmlMap(data, GetEmitterOptions());
  }
  else if (extension == ".yml" || extension == ".yaml") {
    const auto data = ConvertDocumentToIR(document);
    EmitYamlMap(data);
  }
  else {
    CENTURION_LOG_ERROR("Failed to save map document due to invalid extension: %s",
                        extension.string().c_str());
  }

  TACTILE_PROFILE_END("IO::SaveMapDocument()")
}

}  // namespace Tactile::IO
