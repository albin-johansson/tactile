#include "session.hpp"

#ifdef _MSC_VER
#pragma warning(push, 1)
#endif  // _MSC_VER

#include <session.pb.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif  // _MSC_VER

#include <centurion.hpp>  // ...
#include <filesystem>     // absolute
#include <fstream>        // ifstream, ofstream
#include <ios>            // ios

#include "aliases/json.hpp"
#include "core/model.hpp"
#include "directories.hpp"
#include "parsing/map_parser.hpp"
#include "parsing/to_map_document.hpp"
#include "saving/common_saving.hpp"

namespace Tactile {
namespace {

constexpr int format_version = 1;

inline const auto file_path = GetPersistentFileDir() / "session.bin";

}  // namespace

void RestoreLastSession(Model& model)
{
  ProtoBuf::Session session;

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
  ProtoBuf::Session session;
  for (const auto& [id, document] : model) {
    if (!document->path.empty()) {
      const auto documentPath = std::filesystem::absolute(document->path);
      session.add_files(IO::ConvertToForwardSlashes(documentPath));
    }
  }

  std::ofstream stream{file_path, std::ios::out | std::ios::trunc | std::ios::binary};
  if (!session.SerializeToOstream(&stream)) {
    CENTURION_LOG_WARN("Failed to save session binary file!");
  }
}

}  // namespace Tactile
