// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <pugixml.hpp>

#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"

namespace tactile::tiled::tmx {

TACTILE_TMX_FORMAT_API auto emit_external_tileset_file(
    const FilePath& tileset_path,
    const ir::Tileset& tileset,
    const SaveFormatWriteOptions& options) -> Result<void>;

TACTILE_TMX_FORMAT_API void append_external_tileset_node(
    pugi::xml_node root_node,
    const ir::TilesetRef& tileset_ref,
    const FilePath& tileset_path);

TACTILE_TMX_FORMAT_API void append_embedded_tileset_node(
    pugi::xml_node root_node,
    const ir::TilesetRef& tileset_ref);

TACTILE_TMX_FORMAT_API void append_tileset_node(pugi::xml_node root_node,
                                                const ir::TilesetRef& tileset_ref,
                                                const SaveFormatWriteOptions& options);

}  // namespace tactile::tiled::tmx
