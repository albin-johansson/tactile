#pragma once

#include <centurion.hpp>

#include "meta/build.hpp"
#include "tactile.hpp"

namespace tactile {

#if TACTILE_PLATFORM_OSX

#define TACTILE_PRIMARY_MOD "Cmd"
#define TACTILE_SECONDARY_MOD "Option"

inline constexpr cen::key_mod gPrimaryModifier = cen::key_mod::lgui;

#else

#define TACTILE_PRIMARY_MOD "Ctrl"
#define TACTILE_SECONDARY_MOD "Alt"

inline constexpr cen::key_mod gPrimaryModifier = cen::key_mod::lctrl;

#endif  // TACTILE_PLATFORM_OSX

inline constexpr cen::key_mod gSecondaryModifier = cen::key_mod::lalt;

}  // namespace tactile
