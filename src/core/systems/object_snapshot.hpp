#pragma once

#include "core/components/object.hpp"
#include "property_context_snapshot.hpp"

namespace Tactile::Sys {

struct ObjectSnapshot final
{
  Object core;
  PropertyContextSnapshot context;
};

}  // namespace Tactile::Sys
