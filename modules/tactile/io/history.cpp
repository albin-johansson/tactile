#include "history.hpp"

#include <algorithm>   // find
#include <filesystem>  // exists
#include <fstream>     // ifstream, ofstream
#include <ios>         // ios
#include <utility>     // move

#include <history.pb.h>

#include <tactile-base/convert_to_forward_slashes.hpp>
#include <tactile-base/tactile_std.hpp>

#include "directories.hpp"

namespace Tactile {
namespace {

constexpr int format_version = 1;
constexpr usize max_size = 10;

inline const auto file_path = GetPersistentFileDir() / "history.bin";

/* We store paths as strings because that makes displaying them in menus
   _much_ easier (and faster) */
inline Maybe<std::string> last_closed_file;
inline std::deque<std::string> history;

}  // namespace

void LoadFileHistory()
{
  std::ifstream stream{file_path, std::ios::in | std::ios::binary};

  Proto::History h;
  if (h.ParseFromIstream(&stream)) {
    if (h.has_last_opened_file()) {
      last_closed_file = h.last_opened_file();
    }

    for (auto file : h.files()) {
      history.push_back(std::move(file));
    }
  }
  else {
    cen::log::warn("Failed to parse history file!");
  }
}

void SaveFileHistory()
{
  Proto::History h;

  if (last_closed_file) {
    h.set_last_opened_file(*last_closed_file);
  }

  for (const auto& path : history) {
    h.add_files(path);
  }

  {
    std::ofstream stream{file_path, std::ios::out | std::ios::trunc | std::ios::binary};
    if (!h.SerializeToOstream(&stream)) {
      cen::log::error("Failed to save file history!");
    }
  }
}

void ClearFileHistory()
{
  history.clear();
}

void AddFileToHistory(const std::filesystem::path& path)
{
  if (std::ranges::find(history, path) == history.end()) {
    history.push_back(ConvertToForwardSlashes(path));
  }

  if (history.size() > max_size) {
    history.pop_front();
  }
}

void SetLastClosedFile(const std::filesystem::path& path)
{
  last_closed_file = ConvertToForwardSlashes(path);
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
