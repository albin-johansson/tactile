#include "session.hpp"

#include <centurion.hpp>  // ...
#include <filesystem>     // exists
#include <fstream>        // ifstream
#include <json.hpp>       // json
#include <string>         // string
#include <utility>        // move

#include "core/model.hpp"
#include "directories.hpp"
#include "parsing/map_parser.hpp"
#include "parsing/to_map_document.hpp"
#include "saving/common.hpp"
#include "saving/json/save_json.hpp"

namespace Tactile {
namespace {

constexpr int format_version = 1;

inline const auto path = GetPersistentFileDir() / "session.json";

}  // namespace

void RestoreLastSession(Model& model)
{
  if (std::filesystem::exists(path))
  {
    std::ifstream stream{path};

    nlohmann::json json;
    stream >> json;

    for (const auto& [key, value] : json.at("maps").items())
    {
      IO::MapParser parser{value.get<std::string>()};
      if (parser)
      {
        model.AddMap(IO::ToMapDocument(parser.GetData()));
      }
      else
      {
        CENTURION_LOG_ERROR("Failed to restore a map from previous session!");
      }
    }
  }
  else
  {
    CENTURION_LOG_WARN("Could not locate a session JSON file!");
  }
}

void SaveSession(const Model& model)
{
  auto json = nlohmann::json::object();
  auto array = nlohmann::json::array();

  for (const auto& [id, document] : model)
  {
    if (document->HasPath())
    {
      const auto documentPath = document->GetAbsolutePath();
      array += IO::ConvertToForwardSlashes(documentPath);
    }
  }

  json["maps"] = std::move(array);
  json["format_version"] = format_version;

  IO::SaveJson(json, path);
}

}  // namespace Tactile
