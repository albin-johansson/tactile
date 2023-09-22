// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/context/metadata.hpp"

#include <utility>  // move

#include "tactile/core/container/lookup.hpp"

namespace tactile {

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

auto Metadata::get_name() const -> const String&
{
  return mName;
}

auto Metadata::property_count() const -> usize
{
  return mProperties.size();
}

}  // namespace tactile
