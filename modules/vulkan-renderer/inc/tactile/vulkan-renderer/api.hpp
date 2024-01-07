// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/prelude.hpp"

#ifdef TACTILE_BUILDING_VULKAN_RENDERER
  #define TACTILE_VULKAN_API TACTILE_DLL_EXPORT
#else
  #define TACTILE_VULKAN_API TACTILE_DLL_IMPORT
#endif
