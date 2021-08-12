#pragma once

#include <concepts>        // invocable
#include <rune/everything.hpp>  // vector_map

#include "abstract_layer.hpp"
#include "aliases/ints.hpp"
#include "aliases/object_id.hpp"
#include "object.hpp"

namespace Tactile {

/**
 * \brief Represents a layer that only contains map objects.
 *
 * \ingroup core
 */
class ObjectLayer final : public ALayer
{
 public:
  using storage_type = rune::vector_map<object_id, Object>;
  using const_iterator = storage_type::const_iterator;

  ObjectLayer();

  [[nodiscard]] auto Clone() const -> SharedLayer override;

  template <std::invocable<object_id, const Object&> T>
  void Each(T&& callable) const
  {
    for (const auto& [id, object] : mObjects)
    {
      callable(id, object);
    }
  }

  /**
   * \brief Adds an object to the layer.
   *
   * \pre `id` mustn't be associated with an existing object in the layer.
   *
   * \param id a unique ID that will be associated with the object.
   * \param object the object that will be added to the layer.
   */
  void AddObject(object_id id, Object object);

  /**
   * \brief Adds a point object to the layer.
   *
   * \pre `id` mustn't be associated with an existing object in the layer.
   *
   * \param id a unique ID that will be associated with the object.
   * \param x the x-coordinate of the point.
   * \param y the y-coordinate of the point.
   */
  void AddPoint(object_id id, float x, float y);

  /**
   * \brief Adds a rectangle object to the layer.
   *
   * \pre `id` mustn't be associated with an existing object in the layer.
   *
   * \param id a unique ID that will be associated with the object.
   * \param x the x-coordinate of the rectangle.
   * \param y the y-coordinate of the rectangle.
   * \param width the width of the rectangle.
   * \param height the height of the rectangle.
   */
  void AddRectangle(object_id id, float x, float y, float width, float height);

  /**
   * \brief Removes an object from the layer.
   *
   * \pre `id` must be associated with an existing object.
   *
   * \param id the ID associated with the object that will be removed.
   */
  void RemoveObject(object_id id);

  /**
   * \brief Indicates whether or not the layer contains the specified object.
   *
   * \param id the ID of the object to look for.
   *
   * \return `true` if there is an object in the layer with the specified ID;
   * `false` otherwise.
   */
  [[nodiscard]] auto HasObject(object_id id) const -> bool;

  /**
   * \brief Returns the object associated with the specified ID.
   *
   * \note This function throws if there is no object associated with the
   * supplied ID.
   *
   * \param id the ID associated with the desired object.
   *
   * \return the object associated with the ID.
   */
  [[nodiscard]] auto GetObject(object_id id) -> Object&;

  /// \copydoc GetObject()
  [[nodiscard]] auto GetObject(object_id id) const -> const Object&;

  /**
   * \brief Returns the current amount of objects in the layer.
   *
   * \return the number of objects in the layer.
   */
  [[nodiscard]] auto GetObjectCount() const noexcept -> usize;

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return mObjects.begin();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return mObjects.end();
  }

 private:
  storage_type mObjects;
};

}  // namespace Tactile
