// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/filesystem.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-json-format/api.hpp"
#include "tactile/tiled-json-format/common.hpp"

namespace tactile::tiled::tmj {

[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto parse_map(const JSON& json,
                                      String filename,
                                      const SaveFormatReadOptions& options)
    -> Result<ir::Map>;

}  // namespace tactile::tiled::tmj
