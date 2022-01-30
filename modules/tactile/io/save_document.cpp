#include "save_document.hpp"

#include <filesystem>  // absolute

#include "assert.hpp"
#include "convert_document_to_ir.hpp"
#include "emitter/emit_info.hpp"
#include "emitter/json_emitter.hpp"
#include "emitter/yaml_emitter.hpp"
#include "logging.hpp"
#include "preferences.hpp"
#include "profile.hpp"

namespace tactile {

void SaveDocument(const Document& document)
{
  TACTILE_ASSERT(!document.path.empty());
  TACTILE_PROFILE_START

  const auto path = std::filesystem::absolute(document.path);
  log_info("Trying to save map to {}", path);

  emitter::emit_info info{path, convert_document_to_ir(document)};

  // TODO XML

  const auto ext = path.extension();
  if (ext == ".yaml" || ext == ".yml") {
    emit_yaml_map(info);
  }
  else if (ext == ".json") {
    emit_json_map(info);
  }
  else {
    log_error("Unsupported file extension {}", ext);
  }

  TACTILE_PROFILE_END("Emitted document")
}

}  // namespace tactile
