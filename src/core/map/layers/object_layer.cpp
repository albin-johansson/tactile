#include "object_layer.hpp"

#include <cassert>  // assert
#include <utility>  // move

namespace Tactile {

ObjectLayer::ObjectLayer() : ALayer{LayerType::ObjectLayer}
{
  SetName("Object layer");
}

auto ObjectLayer::Clone() const -> SharedLayer
{
  return std::make_shared<ObjectLayer>(*this);
}

void ObjectLayer::AddObject(const object_id id, Object object)
{
  assert(!HasObject(id));
  mObjects.emplace(id, std::move(object));
}

void ObjectLayer::AddPoint(const object_id id, const float x, const float y)
{
  assert(!HasObject(id));
  auto& [key, point] = mObjects.emplace(id, ObjectType::Point);
  point.SetX(x);
  point.SetY(y);
}

void ObjectLayer::AddRectangle(const object_id id,
                               const float x,
                               const float y,
                               const float width,
                               const float height)
{
  assert(!HasObject(id));
  auto& [key, rect] = mObjects.emplace(id, ObjectType::Rectangle);
  rect.SetX(x);
  rect.SetY(y);
  rect.SetWidth(width);
  rect.SetHeight(height);
}

void ObjectLayer::RemoveObject(const object_id id)
{
  assert(HasObject(id));
  mObjects.erase(id);
}

auto ObjectLayer::HasObject(const object_id id) const -> bool
{
  return mObjects.contains(id);
}

auto ObjectLayer::GetObject(const object_id id) -> Object&
{
  return mObjects.at(id);
}

auto ObjectLayer::GetObject(const object_id id) const -> const Object&
{
  return mObjects.at(id);
}

auto ObjectLayer::GetObjectCount() const noexcept -> usize
{
  return mObjects.size();
}

}  // namespace Tactile
