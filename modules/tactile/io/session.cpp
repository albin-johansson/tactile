#include "session.hpp"

#include <filesystem>  // absolute
#include <fstream>     // ifstream, ofstream
#include <ios>         // ios

#include <centurion.hpp>  // ...
#include <session.pb.h>

#include <tactile-base/convert_to_forward_slashes.hpp>

#include "directories.hpp"
#include "editor/model.hpp"
#include "json.hpp"
#include "map_parser.hpp"
#include "to_map_document.hpp"

namespace Tactile {
namespace {

constexpr int format_version = 1;

inline const auto file_path = GetPersistentFileDir() / "session.bin";

}  // namespace

void RestoreLastSession(Model& model)
{
  Proto::Session session;

  std::ifstream stream{file_path, std::ios::in | std::ios::binary};
  if (session.ParseFromIstream(&stream)) {
    for (const auto& file : session.files()) {
      IO::MapParser parser{file};
      if (parser) {
        model.AddMap(IO::ToMapDocument(parser.GetData()));
      }
      else {
        CENTURION_LOG_ERROR("Failed to restore a map from previous session!");
      }
    }
  }
  else {
    CENTURION_LOG_WARN("Failed to parse binary session file!");
  }
}

void SaveSession(const Model& model)
{
  Proto::Session session;
  for (const auto& [id, document] : model) {
    if (!document->path.empty()) {
      const auto documentPath = std::filesystem::absolute(document->path);
      session.add_files(ConvertToForwardSlashes(documentPath));
    }
  }

  std::ofstream stream{file_path, std::ios::out | std::ios::trunc | std::ios::binary};
  if (!session.SerializeToOstream(&stream)) {
    CENTURION_LOG_WARN("Failed to save session binary file!");
  }
}

}  // namespace Tactile
