#pragma once

namespace tactile {

struct export_options final
{
  bool generateDefaults;
  bool embedTilesets;
};

[[nodiscard]] auto make_export_options() -> export_options;

}  // namespace tactile
