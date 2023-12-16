// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-json-format/api.hpp"
#include "tactile/tiled-json-format/common/json.hpp"

namespace tactile::tiled::tmj {

[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto emit_map(const ir::Map& map,
                                     const SaveFormatWriteOptions& options)
    -> Result<JSON>;

}  // namespace tactile::tiled::tmj
