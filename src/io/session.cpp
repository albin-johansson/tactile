#include "session.hpp"

#include <centurion.hpp>  // ...
#include <filesystem>     // exists, absolute
#include <string>         // string
#include <utility>        // move

#include "aliases/json.hpp"
#include "core/model.hpp"
#include "directories.hpp"
#include "parsing/json/read_json.hpp"
#include "parsing/map_parser.hpp"
#include "parsing/to_map_document.hpp"
#include "saving/common_saving.hpp"
#include "saving/json/save_json.hpp"

namespace Tactile {
namespace {

constexpr int format_version = 1;

inline const auto file_path = GetPersistentFileDir() / "session.json";

}  // namespace

void RestoreLastSession(Model& model)
{
  if (std::filesystem::exists(file_path)) {
    const auto json = ReadJson(file_path);

    if (!json) {
      CENTURION_LOG_WARN("Failed to read session JSON file!");
      return;
    }

    for (const auto& [key, value] : json->at("maps").items()) {
      IO::MapParser parser{value.get<std::string>()};
      if (parser) {
        model.AddMap(IO::ToMapDocument(parser.GetData()));
      }
      else {
        CENTURION_LOG_ERROR("Failed to restore a map from previous session!");
      }
    }
  }
  else {
    CENTURION_LOG_WARN("Could not locate a session JSON file!");
  }
}

void SaveSession(const Model& model)
{
  auto json = JSON::object();
  auto array = JSON::array();

  for (const auto& [id, document] : model) {
    if (!document->path.empty()) {
      const auto documentPath = std::filesystem::absolute(document->path);
      array += IO::ConvertToForwardSlashes(documentPath);
    }
  }

  json["maps"] = std::move(array);
  json["format_version"] = format_version;

  IO::SaveJson(json, file_path);
}

}  // namespace Tactile
