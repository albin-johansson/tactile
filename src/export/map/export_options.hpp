#pragma once

namespace tactile {

struct ExportOptions final
{
  bool generateDefaults{};
  bool embedTilesets{};
};

[[nodiscard]] auto MakeExportOptions() -> ExportOptions;

}  // namespace tactile
