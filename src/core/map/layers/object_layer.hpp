#pragma once

#include <concepts>        // invocable
#include <vector_map.hpp>  // vector_map

#include "aliases/ints.hpp"
#include "aliases/object_id.hpp"
#include "layer.hpp"
#include "layer_delegate.hpp"
#include "object.hpp"

namespace Tactile {

/**
 * \class ObjectLayer
 *
 * \brief Represents a layer that only contains map objects.
 *
 * \headerfile object_layer.hpp
 */
class ObjectLayer final : public ILayer
{
 public:
  ObjectLayer();

  /// \name Layer API
  /// \{

  void SetVisible(bool visible) noexcept override;

  void SetOpacity(float opacity) override;

  void SetName(std::string name) override;

  [[nodiscard]] auto GetType() const -> LayerType override;

  [[nodiscard]] auto IsVisible() const -> bool override;

  [[nodiscard]] auto GetOpacity() const noexcept -> float override;

  [[nodiscard]] auto Clone() const -> SharedLayer override;

  /// \} End of layer API

  /// \name Object layer API
  /// \{

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

  /// \} End of object layer API

  /// \name Property API
  /// \{

  void AddProperty(const std::string& name, PropertyType type) override;

  void AddProperty(const std::string& name, const Property& property) override;

  void RemoveProperty(std::string_view name) override;

  void RenameProperty(std::string_view oldName, const std::string& newName) override;

  void SetProperty(std::string_view name, const Property& property) override;

  void ChangePropertyType(std::string_view name, PropertyType type) override;

  [[nodiscard]] auto HasProperty(std::string_view name) const -> bool override;

  [[nodiscard]] auto GetProperty(std::string_view name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperties() const -> const PropertyMap& override;

  [[nodiscard]] auto GetPropertyCount() const -> usize override;

  [[nodiscard]] auto GetName() const -> std::string_view override;

  /// \} End of property API

 private:
  rune::vector_map<object_id, Object> mObjects;
  LayerDelegate mDelegate;
};

}  // namespace Tactile
