#include "tactile/core/context/component.hpp"

#include <utility>  // move

#include "tactile/foundation/container/lookup.hpp"

namespace tactile {

Component::Component(const ComponentID id, AttributeMap attributes)
  : mComponentId {id},
    mAttributes {std::move(attributes)}
{}

auto Component::add_attribute(String name) -> Attribute&
{
  return mAttributes[std::move(name)];
}

auto Component::remove_attribute(const StringView name) -> Maybe<Attribute>
{
  return erase_from(mAttributes, name);
}

auto Component::rename_attribute(const StringView current_name, String new_name) -> bool
{
  if (mAttributes.contains(new_name)) {
    return false;
  }

  if (auto removed_attribute = remove_attribute(current_name)) {
    auto& new_attribute = add_attribute(std::move(new_name));
    new_attribute = std::move(*removed_attribute);

    return true;
  }

  return false;
}

auto Component::get_attribute(const StringView name) -> Attribute&
{
  return lookup_in(mAttributes, name);
}

auto Component::get_attribute(const StringView name) const -> const Attribute&
{
  return lookup_in(mAttributes, name);
}

auto Component::has_attribute(const StringView name) const -> bool
{
  return exists_in(mAttributes, name);
}

auto Component::attribute_count() const -> usize
{
  return mAttributes.size();
}

auto Component::get_id() const -> ComponentID
{
  return mComponentId;
}

}  // namespace tactile
