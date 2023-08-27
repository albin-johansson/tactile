// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"

#ifdef TACTILE_BUILDING_EDITOR
  #define TACTILE_EDITOR_API TACTILE_DLL_EXPORT
#else
  #define TACTILE_EDITOR_API TACTILE_DLL_IMPORT
#endif
