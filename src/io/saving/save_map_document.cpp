#include "save_map_document.hpp"

#include <cassert>  // assert

#include "io/saving/json/save_map_document_as_json.hpp"
#include "io/saving/xml/save_map_document_as_xml.hpp"

namespace Tactile::IO {

void SaveMapDocument(const MapDocument& document)
{
  assert(document.HasPath());

  const auto path = document.GetAbsolutePath();
  CENTURION_LOG_INFO("Saving map document to \"%s\"...", path.string().c_str());

  if (const auto extension = path.extension(); extension == ".json")
  {
    SaveMapDocumentAsJson(document);
  }
  else if (extension == ".tmx")
  {
    SaveMapDocumentAsXml(document);
  }
  else
  {
    CENTURION_LOG_ERROR("Failed to save map document due to invalid extension: %s",
                        extension.string().c_str());
  }
}

}  // namespace Tactile::IO
