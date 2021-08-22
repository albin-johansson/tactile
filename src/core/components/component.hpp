#pragma once

#include <rune/everything.hpp>  // vector_map
#include <string>               // string
#include <variant>              // variant

#include "core/properties/property.hpp"

namespace Tactile {

class Component final
{
 public:
  explicit Component(std::string type);

 private:
  std::string mType;
  rune::vector_map<std::string, Property> mProperties;
};

}  // namespace Tactile
