// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "session.hpp"

#include <spdlog/spdlog.h>

#include "io/directories.hpp"
#include "io/ir/map_document_from_ir.hpp"
#include "io/map/parse/parse_map.hpp"
#include "io/proto/proto.hpp"
#include "io/stream.hpp"
#include "model/document/map_document.hpp"
#include "model/model.hpp"
#include "tactile/core/platform/filesystem.hpp"

namespace tactile {
namespace {

constexpr int kSessionFormatVersion [[maybe_unused]] = 1;

[[nodiscard]] auto get_file_path() -> const Path&
{
  static const auto path = get_persistent_file_dir() / "session.bin";
  return path;
}

}  // namespace

void load_session_from_disk(DocumentModel& model)
{
  proto::Session session;

  auto stream = open_input_stream(get_file_path(), FileType::Binary);
  if (!stream) {
    spdlog::error("Could not open session file");
    return;
  }

  if (session.ParseFromIstream(&stream.value())) {
    for (const auto& file_path : session.files()) {
      const auto ir_map = parse_map(file_path);
      if (ir_map.has_value()) {
        create_map_document_from_ir(*ir_map, file_path, model);
      }
      else {
        spdlog::warn("Failed to restore map from last session!");
      }
    }
  }
  else {
    spdlog::warn("Failed to parse binary session file!");
  }
}

void save_session_to_disk(const DocumentModel& model)
{
  proto::Session session;
  model.each([&](const UUID& document_id) {
    if (model.is_map(document_id)) {
      const auto& map_doc = model.get_map_document(document_id);
      if (map_doc.has_path()) {
        const auto document_path = fs::absolute(map_doc.get_path());
        session.add_files(normalize_path(document_path));
      }
    }
  });

  auto stream = open_output_stream(get_file_path(), FileType::Binary);
  if (stream.has_value()) {
    if (!session.SerializeToOstream(&stream.value())) {
      spdlog::error("Failed to save session file");
    }
  }
  else {
    spdlog::error("Failed to open session file for writing");
  }
}

}  // namespace tactile
