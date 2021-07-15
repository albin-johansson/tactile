#include "save_xml.hpp"

#include <fstream>  // ofstream

namespace Tactile::IO {

void SaveXml(const pugi::xml_document& xml, const std::filesystem::path& path)
{
  std::ofstream stream{path};
  xml.save(stream, " ");
}

}  // namespace Tactile::IO
