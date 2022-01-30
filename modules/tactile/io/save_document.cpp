#include "save_document.hpp"

#include <filesystem>  // absolute

#include "assert.hpp"
#include "emitter/emit_info.hpp"
#include "emitter/yaml/yaml_emitter.hpp"
#include "preferences.hpp"
#include "profile.hpp"

namespace tactile {

void SaveDocument(const Document& document)
{
  TACTILE_ASSERT(!document.path.empty());
  TACTILE_PROFILE_START

  const auto path = std::filesystem::absolute(document.path);
  log_info("Trying to save map to {}", path);

  emitter::EmitInfo info{path, document.registry};
  emit_yaml_map(info);

  TACTILE_PROFILE_END("Emitted document")
}

}  // namespace tactile
