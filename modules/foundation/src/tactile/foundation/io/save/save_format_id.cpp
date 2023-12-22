// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/save/save_format_id.hpp"

#include "tactile/foundation/debug/exception.hpp"

namespace tactile {

auto to_string(const SaveFormatId save_format_id) -> StringView
{
  switch (save_format_id) {
    case SaveFormatId::kUnknown: return "Unknown";
    case SaveFormatId::kTactileYaml: return "Tactile YAML";
    case SaveFormatId::kTiledJson: return "Tiled JSON (TMJ)";
    case SaveFormatId::kTiledXml: return "Tiled XML (TMX)";
    case SaveFormatId::kGodotScene: return "Godot Scene (TSCN)";
  }

  throw Exception {"Invalid save format ID"};
}

}  // namespace tactile