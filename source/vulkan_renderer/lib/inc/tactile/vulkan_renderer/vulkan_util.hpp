// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <vulkan/vulkan.h>

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/vulkan_renderer/api.hpp"

namespace tactile {

[[nodiscard]]
TACTILE_VULKAN_API auto to_string(VkResult result) noexcept -> StringView;

}  // namespace tactile
