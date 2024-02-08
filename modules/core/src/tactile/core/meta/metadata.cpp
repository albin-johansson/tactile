// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/meta/metadata.hpp"

#include <utility>  // move

#include "tactile/foundation/container/lookup.hpp"

namespace tactile::core {

auto Metadata::add_property(String name) -> Attribute&
{
  auto [pair, did_insert] = mProperties.insert_or_assign(std::move(name), Attribute {});
  return pair->second;
}

auto Metadata::remove_property(const StringView name) -> Maybe<Attribute>
{
  return erase_from(mProperties, name);
}

auto Metadata::rename_property(const StringView current_name, String new_name) -> bool
{
  if (auto old_value = remove_property(current_name)) {
    auto& new_value = add_property(std::move(new_name));
    new_value = std::move(*old_value);

    return true;
  }

  return false;
}

auto Metadata::has_property(const StringView name) const -> bool
{
  return exists_in(mProperties, name);
}

auto Metadata::get_property(const StringView name) -> Attribute&
{
  return lookup_in(mProperties, name);
}

auto Metadata::get_property(const StringView name) const -> const Attribute&
{
  return lookup_in(mProperties, name);
}

void Metadata::set_name(String name)
{
  mName = std::move(name);
}

auto Metadata::uuid() const -> const UUID&
{
  return mUUID;
}

auto Metadata::name() const -> const String&
{
  return mName;
}

auto Metadata::property_count() const -> usize
{
  return mProperties.size();
}

auto Metadata::component_count() const -> usize
{
  return mComponents.size();
}

auto Metadata::clone() const -> Metadata
{
  auto other = Metadata {};

  other.mName = mName;
  other.mProperties = mProperties;

  return other;
}

}  // namespace tactile::core
