#pragma once

#include "tactile/core/components/object.hpp"
#include "tactile/core/systems/property_context_snapshot.hpp"

namespace Tactile::Sys {

struct ObjectSnapshot final
{
  Object core;
  PropertyContextSnapshot context;
};

}  // namespace Tactile::Sys
