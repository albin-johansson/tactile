#ifndef RUNE_CORE_INPUT_HPP
#define RUNE_CORE_INPUT_HPP

#include <centurion.hpp>  // keyboard, mouse

namespace rune {

/// \addtogroup core
/// \{

struct input final
{
  cen::keyboard keyboard;
  cen::mouse mouse;
};

/// \} End of group core

}  // namespace rune

#endif  // RUNE_CORE_INPUT_HPP
