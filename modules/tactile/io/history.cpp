#include "history.hpp"

#include <algorithm>   // find_if
#include <filesystem>  // exists
#include <fstream>     // ifstream, ofstream
#include <ios>         // ios
#include <utility>     // move

#include <tactile_def.hpp>
#include <tactile_stdlib.hpp>

#include "directories.hpp"

#include <history.pb.h>

namespace Tactile {
namespace {

constexpr int gFormatVersion = 1;
constexpr usize gMaxSize = 10;

inline const auto gFilePath = GetPersistentFileDir() / "history.bin";

/* We store paths as strings because that makes displaying them in menus
   _much_ easier (and faster) */
inline Maybe<std::string> gLastClosedFile;
inline std::deque<std::string> gHistory;

}  // namespace

void LoadFileHistory()
{
  std::ifstream stream{gFilePath, std::ios::in | std::ios::binary};

  Proto::History h;
  if (h.ParseFromIstream(&stream)) {
    if (h.has_last_opened_file()) {
      gLastClosedFile = h.last_opened_file();
    }

    for (auto file : h.files()) {
      gHistory.push_back(std::move(file));
    }
  }
  else {
    cen::log::warn("Failed to parse history file!");
  }
}

void SaveFileHistory()
{
  Proto::History h;

  if (gLastClosedFile) {
    h.set_last_opened_file(*gLastClosedFile);
  }

  for (const auto& path : gHistory) {
    h.add_files(path);
  }

  {
    std::ofstream stream{gFilePath, std::ios::out | std::ios::trunc | std::ios::binary};
    if (!h.SerializeToOstream(&stream)) {
      cen::log::error("Failed to save file history!");
    }
  }
}

void ClearFileHistory()
{
  gHistory.clear();
}

void AddFileToHistory(const std::filesystem::path& path)
{
  const auto it = std::find_if(gHistory.begin(),
                               gHistory.end(),
                               [&](const std::string& str) { return str == path; });

  if (it == gHistory.end()) {
    gHistory.push_back(ConvertToForwardSlashes(path));

    if (gHistory.size() > gMaxSize) {
      gHistory.pop_front();
    }
  }
}

void SetLastClosedFile(const std::filesystem::path& path)
{
  gLastClosedFile = ConvertToForwardSlashes(path);
  AddFileToHistory(path);
}

auto GetFileHistory() -> const std::deque<std::string>&
{
  return gHistory;
}

auto HasValidLastClosedFile() -> bool
{
  return gLastClosedFile && std::filesystem::exists(*gLastClosedFile);
}

auto GetLastClosedFile() -> const std::string&
{
  return gLastClosedFile.value();
}

}  // namespace Tactile
