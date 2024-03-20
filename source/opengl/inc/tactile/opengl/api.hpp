// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

#ifdef TACTILE_BUILDING_OPENGL_RENDERER
  #define TACTILE_OPENGL_API TACTILE_DLL_EXPORT
#else
  #define TACTILE_OPENGL_API TACTILE_DLL_IMPORT
#endif
