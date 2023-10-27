// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/functional/result.hpp"
#include "tactile/core/io/ir.hpp"
#include "tactile/core/io/save/save_format_options.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/tmj-format/api.hpp"
#include "tactile/tmj-format/common/json.hpp"

namespace tactile::tmj {

[[nodiscard]]
TACTILE_TMJ_API auto emit_map(const ir::Map& map, const SaveFormatWriteOptions& options)
    -> Result<JSON>;

}  // namespace tactile::tmj
