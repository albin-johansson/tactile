// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/prelude.hpp"

#ifdef TACTILE_BUILDING_TACTILE_YML_FORMAT
  #define TACTILE_YML_FORMAT_API TACTILE_DLL_EXPORT
#else
  #define TACTILE_YML_FORMAT_API TACTILE_DLL_IMPORT
#endif
