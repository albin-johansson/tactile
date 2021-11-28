#pragma once

#include <tactile_def.hpp>

#include <SDL.h>

#include "build.hpp"

namespace Tactile {

#if TACTILE_PLATFORM_OSX

#define TACTILE_PRIMARY_MOD "Cmd"
#define TACTILE_SECONDARY_MOD "Option"

inline constexpr uint16 primary_modifier = KMOD_LGUI;

#else

#define TACTILE_PRIMARY_MOD "Ctrl"
#define TACTILE_SECONDARY_MOD "Alt"

inline constexpr uint16 primary_modifier = KMOD_LCTRL;

#endif  // TACTILE_PLATFORM_OSX

inline constexpr uint16 secondary_modifier = KMOD_LALT;

}  // namespace Tactile
