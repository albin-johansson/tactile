#include "ir_tile.hpp"

#include "ir_definitions.hpp"

namespace Tactile::IO {

void SetId(Tile& tile, const int32 id)
{
  tile.id = id;
}

void ReserveAnimationFrames(Tile& tile, const usize n)
{
  tile.animation.reserve(n);
}

void ReserveObjects(Tile& tile, const usize n)
{
  tile.objects.reserve(n);
}

void ReserveProperties(Tile& tile, const usize n)
{
  tile.properties.reserve(n);
}

auto AddAnimationFrame(Tile& tile) -> AnimationFrame&
{
  return tile.animation.emplace_back();
}

auto AddObject(Tile& tile) -> Object&
{
  return tile.objects.emplace_back();
}

auto AddProperty(Tile& tile) -> Property&
{
  return tile.properties.emplace_back();
}

auto IsWorthSaving(const Tile& tile) -> bool
{
  return !tile.animation.empty() || !tile.properties.empty() || !tile.objects.empty();
}

auto GetId(const Tile& tile) -> int32
{
  return tile.id;
}

auto GetAnimationFrameCount(const Tile& tile) -> usize
{
  return tile.animation.size();
}

auto GetObjectCount(const Tile& tile) -> usize
{
  return tile.objects.size();
}

auto GetPropertyCount(const Tile& tile) -> usize
{
  return tile.properties.size();
}

auto GetAnimationFrame(Tile& tile, const usize index) -> AnimationFrame&
{
  return tile.animation.at(index);
}

auto GetAnimationFrame(const Tile& tile, const usize index) -> const AnimationFrame&
{
  return tile.animation.at(index);
}

auto GetObject(Tile& tile, const usize index) -> Object&
{
  return tile.objects.at(index);
}

auto GetObject(const Tile& tile, const usize index) -> const Object&
{
  return tile.objects.at(index);
}

auto GetProperty(Tile& tile, const usize index) -> Property&
{
  return tile.properties.at(index);
}

auto GetProperty(const Tile& tile, const usize index) -> const Property&
{
  return tile.properties.at(index);
}

}  // namespace Tactile::IO