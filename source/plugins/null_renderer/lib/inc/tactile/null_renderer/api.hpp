// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/basic/macros.hpp"

#ifdef TACTILE_BUILDING_NULL_RENDERER
  #define TACTILE_NULL_RENDERER_API TACTILE_DLL_EXPORT
#else
  #define TACTILE_NULL_RENDERER_API TACTILE_DLL_IMPORT
#endif
