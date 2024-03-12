// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/texture.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/smart_ptr.hpp"

namespace tactile {

[[nodiscard]] auto load_texture(const Path& path) -> Shared<Texture>;

}  // namespace tactile
