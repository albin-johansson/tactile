#include "save_json.hpp"

#include <fstream>  // ofstream
#include <iomanip>  // setw

#include "io/preferences.hpp"

namespace Tactile::IO {

void SaveJson(const nlohmann::json& json, const std::filesystem::path& path)
{
  std::ofstream stream{path};

  if (Prefs::GetHumanReadableOutput())
  {
    stream << std::setw(2);
  }

  stream << json;
}

}  // namespace Tactile::IO
