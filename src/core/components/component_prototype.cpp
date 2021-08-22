#include "component_prototype.hpp"

#include <utility>  // move

namespace Tactile {

auto ComponentPrototype::Instantiate() -> Component
{
  return Component(std::basic_string());
}

void ComponentPrototype::AddEntry(std::string name, const PropertyType type)
{

}

void ComponentPrototype::SetName(std::string name)
{
  mName = std::move(name);
}

auto ComponentPrototype::GetName() const -> const std::string&
{
  return mName;
}

}  // namespace Tactile
