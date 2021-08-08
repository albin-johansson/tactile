#pragma once

#include <string>   // string
#include <utility>  // move

#include "aliases/maybe.hpp"
#include "core/properties/property_context.hpp"
#include "core/properties/property_delegate.hpp"
#include "object_type.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

/**
 * \class Object
 *
 * \brief Represents "objects" that are present in object layers, e.g. points
 * and rectangles.
 *
 * \details Objects are useful for modelling triggers, level portals, spawn
 * points, etc.
 *
 * \headerfile object.hpp
 */
class Object final : public IPropertyContext
{
 public:
  /**
   * \brief Creates an object of the specified type.
   *
   * \param type the type of the object.
   */
  explicit Object(const ObjectType type) : mType{type}
  {}

  /// \name Object API
  /// \{

  /**
   * \brief Sets the x-coordinate of the object.
   *
   * \param x the new x-coordinate.
   */
  void SetX(const float x) noexcept
  {
    mX = x;
  }

  /**
   * \brief Sets the y-coordinate of the object.
   *
   * \param y the new y-coordinate.
   */
  void SetY(const float y) noexcept
  {
    mY = y;
  }

  /**
   * \brief Sets the width of the object.
   *
   * \param width the new width.
   */
  void SetWidth(const float width) noexcept
  {
    mWidth = width;
  }

  /**
   * \brief Sets the height of the object.
   *
   * \param height the new height.
   */
  void SetHeight(const float height) noexcept
  {
    mHeight = height;
  }

  /**
   * \brief Sets whether or not the object is visible.
   *
   * \param visible `true` if the object should be visible; `false` otherwise.
   */
  void SetVisible(const bool visible) noexcept
  {
    mVisible = visible;
  }

  /**
   * \brief Sets the name of the object.
   *
   * \param name the new name.
   */
  void SetName(std::string name)
  {
    mProperties.SetName(std::move(name));
  }

  /**
   * \brief Sets the custom user-defined type associated with the object.
   *
   * \param type the custom type name; `nothing` to remove the custom type.
   */
  void SetCustomType(Maybe<std::string> type)
  {
    mCustomType = std::move(type);
  }

  /**
   * \brief Returns the current x-coordinate of the object.
   *
   * \return the x-coordinate of the object.
   */
  [[nodiscard]] auto GetX() const noexcept -> float
  {
    return mX;
  }

  /**
   * \brief Returns the current y-coordinate of the object.
   *
   * \return the y-coordinate of the object.
   */
  [[nodiscard]] auto GetY() const noexcept -> float
  {
    return mY;
  }

  /**
   * \brief Returns the current width of the object.
   *
   * \return the width of the object.
   */
  [[nodiscard]] auto GetWidth() const noexcept -> float
  {
    return mWidth;
  }

  /**
   * \brief Returns the current height of the object.
   *
   * \return the height of the object.
   */
  [[nodiscard]] auto GetHeight() const noexcept -> float
  {
    return mHeight;
  }

  /**
   * \brief Indicates whether or not the object is visible.
   *
   * \return `true` if the object is visible; `false` otherwise.
   */
  [[nodiscard]] auto IsVisible() const noexcept -> bool
  {
    return mVisible;
  }

  /**
   * \brief Returns the type associated with the object.
   *
   * \return the type of the object.
   */
  [[nodiscard]] auto GetType() const noexcept -> ObjectType
  {
    return mType;
  }

  /**
   * \brief Returns the custom user-defined type of the object, if present.
   *
   * \return the name of the user-defined type; `nothing` if there is none.
   */
  [[nodiscard]] auto GetCustomType() const -> Maybe<std::string>
  {
    return mCustomType;
  }

  /**
   * \brief Indicates whether or not the object is represented by a point.
   *
   * \return `true` if the object is a point; `false` otherwise.
   */
  [[nodiscard]] auto IsPoint() const noexcept -> bool
  {
    return GetType() == ObjectType::Point;
  }

  /**
   * \brief Indicates whether or not the object is represented by a rectangle.
   *
   * \return `true` if the object is a rectangle; `false` otherwise.
   */
  [[nodiscard]] auto IsRectangle() const noexcept -> bool
  {
    return GetType() == ObjectType::Rectangle;
  }

  /**
   * \brief Indicates whether or not the object is represented by an ellipse.
   *
   * \return `true` if the object is an ellipse; `false` otherwise.
   */
  [[nodiscard]] auto IsEllipse() const noexcept -> bool
  {
    return GetType() == ObjectType::Ellipse;
  }

  /// \} End of object API

  /// \name Property API
  /// \{

  void AddProperty(std::string name, PropertyType type) override;

  void AddProperty(std::string name, const Property& property) override;

  void RemoveProperty(std::string_view name) override;

  void RenameProperty(std::string_view oldName, std::string newName) override;

  void SetProperty(std::string_view name, const Property& property) override;

  void ChangePropertyType(std::string name, PropertyType type) override;

  [[nodiscard]] auto HasProperty(std::string_view name) const -> bool override;

  [[nodiscard]] auto GetProperty(std::string_view name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperties() const -> const PropertyMap& override;

  [[nodiscard]] auto GetPropertyCount() const -> usize override;

  [[nodiscard]] auto GetName() const -> const std::string& override;

  /// \} End of property API

 private:
  float mX{};
  float mY{};
  float mWidth{};
  float mHeight{};
  ObjectType mType;
  Maybe<std::string> mCustomType;
  PropertyDelegate mProperties;
  bool mVisible{true};
};

/// \} End of group core

}  // namespace Tactile
