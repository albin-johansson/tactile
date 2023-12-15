// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/tiled_xml_format.hpp"

#include "tactile/foundation/debug/generic_error.hpp"

namespace tactile {

auto TiledXmlFormat::load_map(const FilePath& map_path,
                              const SaveFormatReadOptions& options) const
    -> Result<ir::Map>
{
  (void) map_path;
  (void) options;
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto TiledXmlFormat::load_tileset(const FilePath& tileset_path,
                                  const SaveFormatReadOptions& options) const
    -> Result<ir::Tileset>
{
  (void) tileset_path;
  (void) options;
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto TiledXmlFormat::save_map(const FilePath& map_path,
                              const ir::Map& map,
                              const SaveFormatWriteOptions& options) const -> Result<void>
{
  (void) map_path;
  (void) map;
  (void) options;
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto TiledXmlFormat::save_tileset(const FilePath& tileset_path,
                                  const ir::Tileset& tileset,
                                  const SaveFormatWriteOptions& options) const
    -> Result<void>
{
  (void) tileset_path;
  (void) tileset;
  (void) options;
  return unexpected(make_generic_error(GenericError::kUnsupported));
}

auto TiledXmlFormat::is_valid_extension(const NativeStringView extension) const -> bool
{
  return extension == TACTILE_NATIVE_STR(".xml") ||
         extension == TACTILE_NATIVE_STR(".tmx");
}

}  // namespace tactile
