// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/prelude.hpp"

#ifdef TACTILE_BUILDING_TILED_XML_FORMAT
  #define TACTILE_TMX_FORMAT_API TACTILE_DLL_EXPORT
#else
  #define TACTILE_TMX_FORMAT_API TACTILE_DLL_IMPORT
#endif