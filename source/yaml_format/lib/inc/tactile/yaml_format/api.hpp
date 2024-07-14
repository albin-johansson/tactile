// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

#ifdef TACTILE_BUILDING_YAML_FORMAT
  #define TACTILE_YAML_FORMAT_API TACTILE_DLL_EXPORT
#else
  #define TACTILE_YAML_FORMAT_API TACTILE_DLL_IMPORT
#endif
