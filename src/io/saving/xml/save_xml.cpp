#include "save_xml.hpp"

#include <fstream>  // ofstream

#include "io/preferences.hpp"

namespace Tactile::IO {

void SaveXml(const pugi::xml_document& xml, const std::filesystem::path& path)
{
  std::ofstream stream{path};

  if (Prefs::GetHumanReadableOutput())
  {
    xml.save(stream, " ");
  }
  else
  {
    xml.save(stream, "");
  }
}

}  // namespace Tactile::IO
