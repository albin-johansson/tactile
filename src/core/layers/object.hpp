#pragma once

#include <QString>  // QString

#include "maybe.hpp"
#include "object_type.hpp"
#include "property_delegate.hpp"
#include "property_manager.hpp"

namespace tactile::core {

/**
 * \class Object
 *
 * \brief Represents "objects" that are present in object layers, e.g. points
 * and rectangles.
 *
 * \details Objects are useful for modelling triggers, level portals, spawn
 * points, etc.
 *
 * \since 0.2.0
 *
 * \headerfile object.hpp
 */
class Object final : public IPropertyManager
{
 public:
  /**
   * \brief Creates an object of the specified type.
   *
   * \param type the type of the object.
   *
   * \since 0.2.0
   */
  explicit Object(ObjectType type);

  /**
   * \brief Sets the x-coordinate of the object.
   *
   * \param x the new x-coordinate of the object.
   *
   * \since 0.2.0
   */
  void SetX(double x) noexcept;

  /**
   * \brief Sets the y-coordinate of the object.
   *
   * \param y the new y-coordinate of the object.
   *
   * \since 0.2.0
   */
  void SetY(double y) noexcept;

  /**
   * \brief Sets the width of the object.
   *
   * \param width the new width of the object.
   *
   * \since 0.2.0
   */
  void SetWidth(double width) noexcept;

  /**
   * \brief Sets the height of the object.
   *
   * \param height the new height of the object.
   *
   * \since 0.2.0
   */
  void SetHeight(double height) noexcept;

  /**
   * \brief Sets whether or not the object is visible.
   *
   * \param visible `true` if the object should be visible; `false` otherwise.
   *
   * \since 0.2.0
   */
  void SetVisible(bool visible) noexcept;

  /**
   * \brief Sets the name of the object.
   *
   * \param name the new name of the object.
   *
   * \since 0.2.0
   */
  void SetName(const QString& name);

  /**
   * \brief Sets the custom user-defined type associated with the object.
   *
   * \param type the name of the custom user type; `std::nullopt` to remove the
   * custom type.
   *
   * \since 0.2.0
   */
  void SetCustomType(maybe<QString> type);

  /**
   * \brief Returns the current x-coordinate of the object.
   *
   * \return the x-coordinate of the object.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto X() const noexcept -> double;

  /**
   * \brief Returns the current y-coordinate of the object.
   *
   * \return the y-coordinate of the object.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto Y() const noexcept -> double;

  /**
   * \brief Returns the current width of the object.
   *
   * \return the width of the object.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto Width() const noexcept -> double;

  /**
   * \brief Returns the current height of the object.
   *
   * \return the height of the object.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto Height() const noexcept -> double;

  /**
   * \brief Indicates whether or not the object is visible.
   *
   * \return `true` if the object is visible; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto IsVisible() const noexcept -> bool;

  /**
   * \brief Returns the current name of the object.
   *
   * \return the name of the object.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto Name() const -> QString;

  /**
   * \brief Returns the type associated with the object.
   *
   * \return the type of the object.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto Type() const noexcept -> ObjectType;

  /**
   * \brief Returns the custom user-defined type of the object, if present.
   *
   * \return the name of the user-defined type; `std::nullopt` if there is none.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto CustomType() const -> maybe<QString>;

  /**
   * \brief Indicates whether or not the object is represented by a point.
   *
   * \note This is a convenience function that calls `Type()` behind-the-scenes.
   *
   * \return `true` if the object is a point; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto IsPoint() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the object is represented by a rectangle.
   *
   * \note This is a convenience function that calls `Type()` behind-the-scenes.
   *
   * \return `true` if the object is a rectangle; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto IsRectangle() const noexcept -> bool;

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

 private:
  double mX{};
  double mY{};
  double mWidth{};
  double mHeight{};
  ObjectType mType;
  maybe<QString> mCustomType;
  QString mName;
  property_delegate mProperties;
  bool mVisible{true};
};

}  // namespace tactile::core
