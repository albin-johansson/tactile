#include "fancy_tile.hpp"

#include <memory>   // make_unique
#include <utility>  // move

namespace Tactile {

FancyTile::FancyTile() : mProperties{std::make_unique<PropertyDelegate>()}
{}

void FancyTile::SetAnimation(TileAnimation animation)
{
  mAnimation = std::move(animation);
}

auto FancyTile::GetAnimation() -> Maybe<TileAnimation>&
{
  return mAnimation;
}

auto FancyTile::GetAnimation() const -> const Maybe<TileAnimation>&
{
  return mAnimation;
}

void FancyTile::AddProperty(std::string name, const PropertyType type)
{
  mProperties->AddProperty(std::move(name), type);
}

void FancyTile::AddProperty(std::string name, const Property& property)
{
  mProperties->AddProperty(std::move(name), property);
}

void FancyTile::RemoveProperty(const std::string_view name)
{
  mProperties->RemoveProperty(name);
}

void FancyTile::RenameProperty(const std::string_view oldName, std::string newName)
{
  mProperties->RenameProperty(oldName, std::move(newName));
}

void FancyTile::SetProperty(const std::string_view name, const Property& property)
{
  mProperties->SetProperty(name, property);
}

void FancyTile::ChangePropertyType(std::string name, const PropertyType type)
{
  mProperties->ChangePropertyType(std::move(name), type);
}

auto FancyTile::HasProperty(const std::string_view name) const -> bool
{
  return mProperties->HasProperty(name);
}

auto FancyTile::GetProperty(const std::string_view name) const -> const Property&
{
  return mProperties->GetProperty(name);
}

auto FancyTile::GetProperties() const -> const PropertyMap&
{
  return mProperties->GetProperties();
}

auto FancyTile::GetPropertyCount() const -> usize
{
  return mProperties->GetPropertyCount();
}

auto FancyTile::GetName() const -> const std::string&
{
  return mProperties->GetName();
}

}  // namespace Tactile
