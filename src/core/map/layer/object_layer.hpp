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
 * \class object_layer
 *
 * \brief Represents a layer that only contains map objects.
 *
 * \since 0.2.0
 *
 * \headerfile object_layer.hpp
 */
class object_layer final : public layer
{
 public:
  object_layer();

  /// \name Layer API

  void set_visible(bool visible) noexcept override;

  void set_opacity(double opacity) override;

  void set_name(QString name) override;

  [[nodiscard]] auto type() const -> layer_type override;

  [[nodiscard]] auto visible() const noexcept -> bool override;

  [[nodiscard]] auto opacity() const noexcept -> double override;

  [[nodiscard]] auto name() const -> const QString& override;

  [[nodiscard]] auto clone() const -> shared<layer> override;

  /// \}

  /// \name Property API
  /// \{

  void add_property(const QString& name, property_type type) override;

  void add_property(const QString& name, const property& property) override;

  void remove_property(const QString& name) override;

  void rename_property(const QString& oldName, const QString& newName) override;

  void set_property(const QString& name, const property& property) override;

  void change_property_type(const QString& name,
                            core::property_type type) override;

  [[nodiscard]] auto get_property(const QString& name) const
      -> const property& override;

  [[nodiscard]] auto get_property(const QString& name) -> property& override;

  [[nodiscard]] auto has_property(const QString& name) const -> bool override;

  [[nodiscard]] auto property_count() const noexcept -> int override;

  [[nodiscard]] auto properties() const -> const property_map& override;

  /// \} End of property API

  /// \name Object layer API
  /// \{

  template <std::invocable<object_id, const object&> T>
  void each_object(T&& callable) const
  {
    for (const auto& [id, object] : m_objects) {
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
  void add_object(object_id id, object obj);

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
  void add_point(object_id id, double x, double y);

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
  void add_rectangle(object_id id,
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
  void remove_object(object_id id);

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
  [[nodiscard]] auto has_object(object_id id) const -> bool;

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
  [[nodiscard]] auto get_object(object_id id) -> object&;

  /**
   * \copydoc get_object()
   */
  [[nodiscard]] auto get_object(object_id id) const -> const object&;

  /**
   * \brief Returns the current amount of objects in the layer.
   *
   * \return the number of objects in the layer.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto object_count() const noexcept -> int;

  /// \}

 private:
  vector_map<object_id, object> m_objects;
  layer_delegate m_delegate;
};

}  // namespace tactile::core
