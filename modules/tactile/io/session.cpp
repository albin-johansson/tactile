#include "session.hpp"

#include <filesystem>  // absolute
#include <fstream>     // ifstream, ofstream
#include <ios>         // ios

#include <tactile_stdlib.hpp>

#include <centurion.hpp>  // ...

#include "create_document_from_ir.hpp"
#include "directories.hpp"
#include "editor/model.hpp"
#include "map_parser.hpp"
#include "logging.hpp"

#include <session.pb.h>

namespace Tactile {
namespace {

constexpr int gFormatVersion = 1;

inline const auto gFilePath = GetPersistentFileDir() / "session.bin";

}  // namespace

void RestoreLastSession(Model& model, TextureManager& textures)
{
  Proto::Session session;

  std::ifstream stream{gFilePath, std::ios::in | std::ios::binary};
  if (session.ParseFromIstream(&stream)) {
    for (const auto& file : session.files()) {
      MapParser parser{file};
      if (parser) {
        model.AddMap(CreateDocumentFromIR(parser.GetData(), textures));
      }
      else {
        LogWarning("Failed to restore map from last session!");
      }
    }
  }
  else {
    LogWarning("Failed to parse binary session file!");
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

  std::ofstream stream{gFilePath, std::ios::out | std::ios::trunc | std::ios::binary};
  if (!session.SerializeToOstream(&stream)) {
    LogError("Failed to save session file!");
  }
}

}  // namespace Tactile
