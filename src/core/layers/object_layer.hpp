#pragma once

#include <concepts>  // invocable

#include "layer.hpp"
#include "layer_delegate.hpp"
#include "object.hpp"
#include "object_id.hpp"
#include "smart_pointers.hpp"
#include "vector_map.hpp"

namespace tactile::core {

/**
 * \class ObjectLayer
 *
 * \brief Represents a layer that only contains map objects.
 *
 * \since 0.2.0
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

  void SetOpacity(double opacity) override;

  void SetName(QString name) override;

  [[nodiscard]] auto Type() const -> LayerType override;

  [[nodiscard]] auto IsVisible() const noexcept -> bool override;

  [[nodiscard]] auto Opacity() const noexcept -> double override;

  [[nodiscard]] auto Name() const -> const QString& override;

  [[nodiscard]] auto Clone() const -> shared<ILayer> override;

  /// \} End of layer API

  /// \name Property API
  /// \{

  void AddProperty(const QString& name, PropertyType type) override;

  void AddProperty(const QString& name, const Property& property) override;

  void RemoveProperty(const QString& name) override;

  void RenameProperty(const QString& oldName, const QString& newName) override;

  void SetProperty(const QString& name, const Property& property) override;

  void ChangePropertyType(const QString& name,
                          core::PropertyType type) override;

  [[nodiscard]] auto GetProperty(const QString& name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperty(const QString& name) -> Property& override;

  [[nodiscard]] auto HasProperty(const QString& name) const -> bool override;

  [[nodiscard]] auto PropertyCount() const noexcept -> int override;

  [[nodiscard]] auto GetProperties() const -> const property_map& override;

  /// \} End of property API

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
   * \param obj the object that will be added to the layer.
   *
   * \since 0.2.0
   */
  void AddObject(object_id id, Object obj);

  /**
   * \brief Adds a point object to the layer.
   *
   * \pre `id` mustn't be associated with an existing object in the layer.
   *
   * \param id a unique ID that will be associated with the object.
   * \param x the x-coordinate of the point.
   * \param y the y-coordinate of the point.
   *
   * \since 0.2.0
   */
  void AddPoint(object_id id, double x, double y);

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
   *
   * \since 0.2.0
   */
  void AddRectangle(object_id id,
                    double x,
                    double y,
                    double width,
                    double height);

  /**
   * \brief Removes an object from the layer.
   *
   * \pre `id` must be associated with an existing object.
   *
   * \param id the ID associated with the object that will be removed.
   *
   * \since 0.2.0
   */
  void RemoveObject(object_id id);

  /**
   * \brief Indicates whether or not the layer contains the specified object.
   *
   * \param id the ID of the object to look for.
   *
   * \return `true` if there is an object in the layer with the specified ID;
   * `false` otherwise.
   *
   * \since 0.2.0
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
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto GetObject(object_id id) -> Object&;

  /// \copydoc GetObject()
  [[nodiscard]] auto GetObject(object_id id) const -> const Object&;

  /**
   * \brief Returns the current amount of objects in the layer.
   *
   * \return the number of objects in the layer.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto ObjectCount() const noexcept -> int;

  /// \} End of object layer API

 private:
  vector_map<object_id, Object> mObjects;
  LayerDelegate mDelegate;
};

}  // namespace tactile::core
