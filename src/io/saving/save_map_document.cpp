#include "save_map_document.hpp"

#include <cassert>     // assert
#include <filesystem>  // absolute

#include <centurion.hpp>  // CENTURION_LOG_{}

#include "core/utils/profile.hpp"
#include "xml/save_map_document_as_xml.hpp"
#include "yaml/save_document_as_yaml.hpp"

#include "tactile-io/json/emitter/save_map_document_as_json.hpp"

namespace Tactile::IO {

void SaveMapDocument(const Document& document)
{
  assert(!document.path.empty());
  TACTILE_PROFILE_START

  const auto path = std::filesystem::absolute(document.path);
  CENTURION_LOG_INFO("Saving map document to \"%s\"", path.string().c_str());

  const auto extension = path.extension();
  if (extension == ".json") {
    SaveMapDocumentAsJson(document);
  }
  else if (extension == ".tmx" || extension == ".xml") {
    SaveMapDocumentAsXml(document);
  }
  else if (extension == ".yml" || extension == ".yaml") {
    SaveDocumentAsYaml(document);
  }
  else {
    CENTURION_LOG_ERROR("Failed to save map document due to invalid extension: %s",
                        extension.string().c_str());
  }

  TACTILE_PROFILE_END("IO::SaveMapDocument()")
}

}  // namespace Tactile::IO
