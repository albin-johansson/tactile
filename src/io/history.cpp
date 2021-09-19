#include "history.hpp"

#include <algorithm>   // find
#include <filesystem>  // exists
#include <utility>     // move

#include "aliases/ints.hpp"
#include "aliases/maybe.hpp"
#include "directories.hpp"
#include "parsing/json/read_json.hpp"
#include "saving/common_saving.hpp"
#include "saving/json/save_json.hpp"

namespace Tactile {
namespace {

constexpr int format_version = 1;
constexpr usize max_size = 10;

inline const auto file_path = GetPersistentFileDir() / "history.json";

/* We store paths as strings because that makes displaying them in menus
   _much_ easier (and faster) */
inline Maybe<std::string> last_closed_file;
inline std::deque<std::string> history;

}  // namespace

void LoadFileHistory()
{
  if (std::filesystem::exists(file_path)) {
    const auto json = ReadJson(file_path.string());

    if (!json) {
      CENTURION_LOG_WARN("Failed to read history JSON file!");
      return;
    }

    if (const auto last = json->at("last_closed_file"); !last.is_null()) {
      last_closed_file = last.get<std::string>();
    }

    for (const auto& [key, value] : json->at("files").items()) {
      auto path = value.get<std::string>();
      if (std::filesystem::exists(path)) {
        history.push_back(std::move(path));
      }
      else {
        CENTURION_LOG_WARN("Read invalid file path from history!");
      }
    }
  }
  else {
    CENTURION_LOG_WARN("Could not locate a history JSON file!");
  }
}

void SaveFileHistory()
{
  auto json = JSON::object();
  json["format_version"] = format_version;

  if (last_closed_file) {
    json["last_closed_file"] = *last_closed_file;
  }
  else {
    json["last_closed_file"] = nullptr;
  }

  auto array = JSON::array();
  for (const auto& path : history) {
    if (std::filesystem::exists(path)) {
      array += path;
    }
  }
  json["files"] = std::move(array);

  IO::SaveJson(json, file_path);
}

void ClearFileHistory()
{
  history.clear();
}

void AddFileToHistory(const std::filesystem::path& path)
{
  if (std::ranges::find(history, path) == history.end()) {
    history.push_back(IO::ConvertToForwardSlashes(path));
  }

  if (history.size() > max_size) {
    history.pop_front();
  }
}

void SetLastClosedFile(const std::filesystem::path& path)
{
  last_closed_file = IO::ConvertToForwardSlashes(path);
  AddFileToHistory(path);
}

auto GetFileHistory() -> const std::deque<std::string>&
{
  return history;
}

auto HasValidLastClosedFile() -> bool
{
  return last_closed_file && std::filesystem::exists(*last_closed_file);
}

auto GetLastClosedFile() -> const std::string&
{
  return last_closed_file.value();
}

}  // namespace Tactile
