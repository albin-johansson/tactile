#pragma once

#include <string>  // string

namespace Tactile {

/// \addtogroup events
/// \{

struct RenamePropertyEvent final
{
  std::string old_name;
  std::string new_name;
};

/// \} End of group events

}  // namespace Tactile
