// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/container/string.hpp"
#include "tactile/core/functional/result.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/io/ir.hpp"
#include "tactile/core/io/save/save_format_options.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/tmj-format/api.hpp"
#include "tactile/tmj-format/common/json.hpp"

namespace tactile::tmj {

[[nodiscard]]
TACTILE_TMJ_API auto parse_map(const JSON& json,
                               String filename,
                               const SaveFormatReadOptions& options) -> Result<ir::Map>;

}  // namespace tactile::tmj
