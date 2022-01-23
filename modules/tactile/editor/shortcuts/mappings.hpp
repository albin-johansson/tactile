#pragma once

#include <SDL.h>
#include <tactile_def.hpp>

#include "build.hpp"

namespace tactile {

#if TACTILE_PLATFORM_OSX

#define TACTILE_PRIMARY_MOD "Cmd"
#define TACTILE_SECONDARY_MOD "Option"

inline constexpr uint16 gPrimaryModifier = KMOD_LGUI;

#else

#define TACTILE_PRIMARY_MOD "Ctrl"
#define TACTILE_SECONDARY_MOD "Alt"

inline constexpr uint16 gPrimaryModifier = KMOD_LCTRL;

#endif  // TACTILE_PLATFORM_OSX

inline constexpr uint16 gSecondaryModifier = KMOD_LALT;

}  // namespace tactile
