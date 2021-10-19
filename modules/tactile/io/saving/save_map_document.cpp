#include "save_map_document.hpp"

#include <cassert>     // assert
#include <filesystem>  // absolute

#include <centurion.hpp>  // CENTURION_LOG_{}

#include <tactile-io/emitter.hpp>

#include "core/utils/profile.hpp"
#include "io/convert_document_to_ir.hpp"
#include "io/saving/xml/save_map_document_as_xml.hpp"

namespace Tactile::IO {

void SaveMapDocument(const Document& document)
{
  assert(!document.path.empty());
  TACTILE_PROFILE_START

  const auto path = std::filesystem::absolute(document.path);
  CENTURION_LOG_INFO("Saving map document to \"%s\"", path.string().c_str());

  const auto extension = path.extension();
  if (extension == ".json") {
    const auto data = ConvertDocumentToIR(document);
    EmitJsonMap(data);
  }
  else if (extension == ".tmx" || extension == ".xml") {
    SaveMapDocumentAsXml(document);
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
