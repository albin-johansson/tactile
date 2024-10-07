// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/godot_tscn_format/godot_scene_format.hpp"

#include <exception>     // exception
#include <system_error>  // make_error_code, errc

#include "tactile/base/document/map_view.hpp"
#include "tactile/base/io/int_parser.hpp"
#include "tactile/godot_tscn_format/gd3_document_converter.hpp"
#include "tactile/godot_tscn_format/gd3_exporter.hpp"
#include "tactile/runtime/logging.hpp"

namespace tactile::godot {
namespace {

[[nodiscard]]
auto _deduce_godot_version(const SaveFormatExtraSettings& settings) -> std::optional<int>
{
  int version {3};

  if (const auto iter = settings.find("godot::version"); iter != settings.end()) {
    if (const auto parsed_version = parse<int>(iter->second)) {
      version = *parsed_version;
    }
  }

  if (version != 3 && version != 4) {
    return std::nullopt;
  }

  return version;
}

}  // namespace

GodotSceneFormat::GodotSceneFormat(IRuntime* runtime)
  : m_runtime {runtime}
{}

auto GodotSceneFormat::load_map(const std::filesystem::path&,
                                const SaveFormatReadOptions&) const
    -> std::expected<ir::Map, std::error_code>
{
  return std::unexpected {std::make_error_code(std::errc::not_supported)};
}

auto GodotSceneFormat::save_map(const IMapView& map,
                                const SaveFormatWriteOptions& options) const
    -> std::expected<void, std::error_code>
{
  try {
    const auto version = _deduce_godot_version(options.extra);

    if (version == 3) {
      Gd3DocumentConverter converter {options};
      map.accept(converter);

      const auto& gd_map = converter.get_map();
      return save_godot3_scene(gd_map, options);
    }

    return std::unexpected {std::make_error_code(std::errc::not_supported)};
  }
  catch (const std::exception& error) {
    log(LogLevel::kError, "Unexpected Godot export error: {}", error.what());
  }

  return std::unexpected {std::make_error_code(std::errc::io_error)};
}

}  // namespace tactile::godot
