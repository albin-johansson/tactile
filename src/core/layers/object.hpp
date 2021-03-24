#pragma once

#include <QString>  // QString

#include "maybe.hpp"
#include "object_type.hpp"
#include "property_delegate.hpp"
#include "property_manager.hpp"

namespace tactile::core {

/**
 * \class object
 *
 * \brief Represents "objects" that are present in object layers, e.g. points
 * and rectangles.
 *
 * \details Objects are useful for modelling triggers, level portals,
 * spawn points, etc.
 *
 * \since 0.2.0
 *
 * \headerfile object.hpp
 */
class object final : public property_manager
{
 public:
  /**
   * \brief Creates an object of the specified type.
   *
   * \param type the type of the object.
   *
   * \since 0.2.0
   */
  explicit object(object_type type);

  /**
   * \brief Sets the x-coordinate of the object.
   *
   * \param x the new x-coordinate of the object.
   *
   * \since 0.2.0
   */
  void set_x(double x) noexcept;

  /**
   * \brief Sets the y-coordinate of the object.
   *
   * \param y the new y-coordinate of the object.
   *
   * \since 0.2.0
   */
  void set_y(double y) noexcept;

  /**
   * \brief Sets the width of the object.
   *
   * \param width the new width of the object.
   *
   * \since 0.2.0
   */
  void set_width(double width) noexcept;

  /**
   * \brief Sets the height of the object.
   *
   * \param width the new height of the object.
   *
   * \since 0.2.0
   */
  void set_height(double height) noexcept;

  /**
   * \brief Sets whether or not the object is visible.
   *
   * \param visible `true` if the object should be visible; `false` otherwise.
   *
   * \since 0.2.0
   */
  void set_visible(bool visible) noexcept;

  /**
   * \brief Sets the name of the object.
   *
   * \param name the new name of the object.
   *
   * \since 0.2.0
   */
  void set_name(const QString& name);

  /**
   * \brief Sets the custom user-defined type associated with the object.
   *
   * \param type the name of the custom user type; `std::nullopt` to remove the
   * custom type.
   *
   * \since 0.2.0
   */
  void set_custom_type(maybe<QString> type);

  /**
   * \brief Returns the current x-coordinate of the object.
   *
   * \return the x-coordinate of the object.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto x() const noexcept -> double;

  /**
   * \brief Returns the current y-coordinate of the object.
   *
   * \return the y-coordinate of the object.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto y() const noexcept -> double;

  /**
   * \brief Returns the current width of the object.
   *
   * \return the width of the object.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto width() const noexcept -> double;

  /**
   * \brief Returns the current height of the object.
   *
   * \return the height of the object.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto height() const noexcept -> double;

  /**
   * \brief Indicates whether or not the object is visible.
   *
   * \return `true` if the object is visible; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto visible() const noexcept -> bool;

  /**
   * \brief Returns the current name of the object.
   *
   * \return the name of the object.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto name() const -> QString;

  /**
   * \brief Returns the type associated with the object.
   *
   * \return the type of the object.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto type() const noexcept -> object_type;

  /**
   * \brief Returns the custom user-defined type of the object, if present.
   *
   * \return the name of the user-defined type; `std::nullopt` if there is none.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto custom_type() const -> maybe<QString>;

  /**
   * \brief Indicates whether or not the object is represented by a point.
   *
   * \note This is a convenience function that calls `type()` behind-the-scenes.
   *
   * \return `true` if the object is a point; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto is_point() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the object is represented by a rectangle.
   *
   * \note This is a convenience function that calls `type()` behind-the-scenes.
   *
   * \return `true` if the object is a rectangle; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto is_rectangle() const noexcept -> bool;

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

 private:
  double m_x{};
  double m_y{};
  double m_width{};
  double m_height{};
  object_type m_type;
  maybe<QString> m_customType;
  QString m_name;
  property_delegate m_properties;
  bool m_visible{true};
};

}  // namespace tactile::core
