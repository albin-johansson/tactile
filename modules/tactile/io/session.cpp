#include "session.hpp"

#include <filesystem>  // path, absolute
#include <fstream>     // ifstream, ofstream
#include <ios>         // ios

#include <tactile_stdlib.hpp>

#include "create_document_from_ir.hpp"
#include "directories.hpp"
#include "editor/model.hpp"
#include "logging.hpp"
#include "map_parser.hpp"
#include <session.pb.h>

namespace tactile {
namespace {

constexpr int gFormatVersion [[maybe_unused]] = 1;

[[nodiscard]] auto get_file_path() -> const std::filesystem::path&
{
  static const auto path = persistent_file_dir() / "session.bin";
  return path;
}

}  // namespace

void restore_last_session(Model& model, TextureManager& textures)
{
  proto::Session session;

  std::ifstream stream{get_file_path(), std::ios::in | std::ios::binary};
  if (session.ParseFromIstream(&stream)) {
    for (const auto& file : session.files()) {
      MapParser parser{file};
      if (parser) {
        model.AddMap(CreateDocumentFromIR(parser.GetData(), textures));
      }
      else {
        log_warning("Failed to restore map from last session!");
      }
    }
  }
  else {
    log_warning("Failed to parse binary session file!");
  }
}

void save_session(const Model& model)
{
  proto::Session session;
  for (const auto& [id, document] : model) {
    if (!document->path.empty()) {
      const auto documentPath = std::filesystem::absolute(document->path);
      session.add_files(ConvertToForwardSlashes(documentPath));
    }
  }

  std::ofstream stream{get_file_path(),
                       std::ios::out | std::ios::trunc | std::ios::binary};
  if (!session.SerializeToOstream(&stream)) {
    log_error("Failed to save session file!");
  }
}

}  // namespace tactile
