#pragma once

#include "core/tool_type.hpp"

namespace tactile::comp {

struct active_tool final
{
  tool_type tool{tool_type::none};
};

}  // namespace tactile
