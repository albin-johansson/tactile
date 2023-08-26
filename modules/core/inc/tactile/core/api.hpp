// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/prelude.hpp"

#ifdef TACTILE_BUILDING_CORE
  #define TACTILE_CORE_API TACTILE_DLL_EXPORT
#else
  #define TACTILE_CORE_API TACTILE_DLL_IMPORT
#endif