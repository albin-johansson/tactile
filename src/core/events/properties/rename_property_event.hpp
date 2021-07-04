#pragma once

#include <string>  // string

namespace Tactile {

struct RenamePropertyEvent final
{
  std::string old_name;
  std::string new_name;
};

}  // namespace Tactile
