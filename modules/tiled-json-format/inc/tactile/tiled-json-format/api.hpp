// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/prelude.hpp"

#ifdef TACTILE_BUILDING_TMJ_PLUGIN
  #define TACTILE_TMJ_API TACTILE_DLL_EXPORT
#else
  #define TACTILE_TMJ_API TACTILE_DLL_IMPORT
#endif
