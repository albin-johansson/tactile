#pragma once

#include <rune/everything.hpp>  // vector_map
#include <string>               // string

#include "component.hpp"
#include "core/properties/property_type.hpp"

namespace Tactile {

class ComponentPrototype final
{
 public:
  [[nodiscard]] auto Instantiate() -> Component;

  void AddEntry(std::string name, PropertyType type);

  void SetName(std::string name);

  [[nodiscard]] auto GetName() const -> const std::string&;

 private:
  std::string mName;
  rune::vector_map<std::string, PropertyType> mEntries;
};

}  // namespace Tactile
