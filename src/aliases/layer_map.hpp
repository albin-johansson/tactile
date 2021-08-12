#pragma once

#include <rune/everything.hpp>  // vector_map

#include "layer_id.hpp"
#include "shared.hpp"

namespace Tactile {

class ILayer;

using layer_map = rune::vector_map<layer_id, Shared<ILayer>>;

}  // namespace Tactile
