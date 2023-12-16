// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <pugixml.hpp>

#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-xml-format/api.hpp"

namespace tactile::tiled::tmx {

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto load_xml_file(const FilePath& path)
    -> Result<pugi::xml_document>;

[[nodiscard]]
TACTILE_TMX_FORMAT_API auto save_xml_file(const pugi::xml_document& document,
                                          const FilePath& path,
                                          int indentation) -> Result<void>;

}  // namespace tactile::tiled::tmx
