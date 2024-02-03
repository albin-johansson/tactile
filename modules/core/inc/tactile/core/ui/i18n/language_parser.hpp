// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

[[nodiscard]]
TACTILE_CORE_API auto parse_language_strings(const FilePath& path)
    -> Result<Vector<String>>;

}  // namespace tactile::core
