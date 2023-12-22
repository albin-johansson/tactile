// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/tiled_xml_format.hpp"

#include "tactile/foundation/debug/generic_error.hpp"
#include "tactile/tiled-xml-format/common.hpp"
#include "tactile/tiled-xml-format/emit/map_emitter.hpp"
#include "tactile/tiled-xml-format/emit/tileset_emitter.hpp"
#include "tactile/tiled-xml-format/parse/map_parser.hpp"
#include "tactile/tiled-xml-format/parse/tileset_parser.hpp"

namespace tactile {

auto TiledXmlFormat::load_map(const FilePath& map_path,
                              const SaveFormatReadOptions& options) const
    -> Result<ir::Map>
{
  return tiled::tmx::load_xml_file(map_path).and_then(
      [&](const pugi::xml_document& document) {
        const auto map_node = document.root().child("map");
        return tiled::tmx::parse_map_node(map_node,
                                          map_path.filename().string(),
                                          options);
      });
}

auto TiledXmlFormat::load_tileset(const FilePath& tileset_path,
                                  const SaveFormatReadOptions& options) const
    -> Result<ir::Tileset>
{
  return tiled::tmx::load_xml_file(tileset_path)
      .and_then([&](const pugi::xml_document& document) {
        const auto tileset_node = document.root().child("tileset");
        return tiled::tmx::parse_embedded_tileset_node(tileset_node, options);
      });
}

auto TiledXmlFormat::save_map(const FilePath& map_path,
                              const ir::Map& map,
                              const SaveFormatWriteOptions& options) const -> Result<void>
{
  pugi::xml_document document {};

  auto root_node = document.root();
  tiled::tmx::append_map_node(root_node, map, options);

  return tiled::tmx::save_xml_file(document, map_path, options.use_indentation ? 2 : 0);
}

auto TiledXmlFormat::save_tileset(const FilePath& tileset_path,
                                  const ir::Tileset& tileset,
                                  const SaveFormatWriteOptions& options) const
    -> Result<void>
{
  return tiled::tmx::emit_external_tileset_file(tileset_path, tileset, options);
}

auto TiledXmlFormat::is_valid_extension(const NativeStringView extension) const -> bool
{
  return extension == TACTILE_NATIVE_STR(".xml") ||
         extension == TACTILE_NATIVE_STR(".tmx") ||
         extension == TACTILE_NATIVE_STR(".tsx");
}

auto TiledXmlFormat::id() const -> SaveFormatId
{
  return SaveFormatId::kTiledXml;
}

}  // namespace tactile
