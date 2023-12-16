// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tiled-json-format/api.hpp"
#include "tactile/tiled-json-format/common/json.hpp"

namespace tactile::tiled::tmj {

[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto parse_property(const JSON& json)
    -> Result<ir::NamedAttribute>;

[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto parse_metadata(const JSON& json, ir::Metadata& meta)
    -> Result<void>;

}  // namespace tactile::tiled::tmj
