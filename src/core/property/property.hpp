#pragma once

#include <QColor>     // QColor
#include <QFileInfo>  // QFileInfo
#include <QString>    // QString
#include <concepts>   // same_as
#include <nenya.hpp>  // strong_type
#include <variant>    // variant, monostate

#include "maybe.hpp"

namespace tactile::core {

namespace tags {
struct object_ref_tag;
}  // namespace tags

using object_ref = nenya::strong_type<int, tags::object_ref_tag>;

template <typename T>
concept IsPropertyType = std::same_as<T, QString> || std::same_as<T, int> ||
    std::same_as<T, double> || std::same_as<T, bool> ||
    std::same_as<T, QColor> || std::same_as<T, QFileInfo> ||
    std::same_as<T, object_ref>;

/**
 * \brief Provides values for all different possible property types.
 *
 * \since 0.2.0
 */
enum class PropertyType
{
  String,    ///< A string property.
  Integer,   ///< An integer property.
  Floating,  ///< A floating-point property.
  Boolean,   ///< A boolean property.
  File,      ///< A file path property.
  Color,     ///< A color property.
  Object     ///< An integer ID property, that refers to a map object.
};

/**
 * \class Property
 *
 * \brief Represents a property associated with maps, layers and other objects.
 *
 * \since 0.2.0
 *
 * \headerfile property.hpp
 */
class Property final
{
 public:
  using value_type = std::variant<std::monostate,
                                  QString,
                                  QColor,
                                  QFileInfo,
                                  object_ref,
                                  int,
                                  double,
                                  bool>;

  /**
   * \brief Creates a property with no value.
   *
   * \since 0.2.0
   */
  Property() = default;

  /**
   * \brief Creates a property.
   *
   * \tparam T the type of the property value.
   *
   * \param value the value of the property.
   *
   * \since 0.2.0
   */
  template <IsPropertyType T>
  /*implicit*/ Property(const T& value)
  {
    mValue.emplace<T>(value);
  }

  /**
   * \brief Resets the state of the property.
   *
   * \post `HasValue()` will return `false` after this function is invoked.
   *
   * \since 0.2.0
   */
  void Reset();

  void SetDefault(PropertyType type);

  /**
   * \brief Sets the value stored in the property.
   *
   * \tparam T the type of the value.
   *
   * \param value the new value of the property.
   *
   * \since 0.2.0
   */
  template <IsPropertyType T>
  void SetValue(const T& value = {})
  {
    mValue.emplace<T>(value);
  }

  /**
   * \brief Indicates whether or not the property holds a value.
   *
   * \return `true` if the property holds a value; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto HasValue() const noexcept -> bool;

  /**
   * \brief Returns the stored string value.
   *
   * \note This function throws if the stored value isn't a color.
   *
   * \return the stored color.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto AsString() const -> const QString&;

  /**
   * \brief Returns the stored integer value.
   *
   * \note This function throws if the stored value isn't an integer.
   *
   * \return the stored integer.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto AsInteger() const -> int;

  /**
   * \brief Returns the stored float value.
   *
   * \note This function throws if the stored value isn't a float.
   *
   * \return the stored floating-point value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto AsFloating() const -> double;

  /**
   * \brief Returns the stored boolean value.
   *
   * \note This function throws if the stored value isn't a boolean.
   *
   * \return the stored boolean value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto AsBoolean() const -> bool;

  /**
   * \brief Returns the stored file path value.
   *
   * \note This function throws if the stored value isn't a file path.
   *
   * \return the stored file path value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto AsFile() const -> const QFileInfo&;

  /**
   * \brief Returns the stored object ID value.
   *
   * \note This function throws if the stored value isn't an object ID.
   *
   * \return the stored object ID value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto AsObject() const -> object_ref;

  /**
   * \brief Returns the stored color value.
   *
   * \note This function throws if the stored value isn't a color.
   *
   * \return the stored color value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto AsColor() const -> const QColor&;

  [[nodiscard]] auto TryAsString() const noexcept -> const QString*
  {
    return TryAs<QString>();
  }

  [[nodiscard]] auto TryAsInteger() const noexcept -> const int*
  {
    return TryAs<int>();
  }

  [[nodiscard]] auto TryAsFloating() const noexcept -> const double*
  {
    return TryAs<double>();
  }

  [[nodiscard]] auto TryAsBoolean() const noexcept -> const bool*
  {
    return TryAs<bool>();
  }

  [[nodiscard]] auto TryAsFile() const noexcept -> const QFileInfo*
  {
    return TryAs<QFileInfo>();
  }

  [[nodiscard]] auto TryAsObject() const noexcept -> const object_ref*
  {
    return TryAs<object_ref>();
  }

  [[nodiscard]] auto TryAsColor() const noexcept -> const QColor*
  {
    return TryAs<QColor>();
  }

  /**
   * \brief Indicates whether or not the stored value is a string.
   *
   * \return `true` if the stored value is a string; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto IsString() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the stored value is an integer.
   *
   * \return `true` if the stored value is an integer; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto IsInteger() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the stored value is a floating-point
   * number.
   *
   * \return `true` if the stored value is a floating-point number; `false`
   * otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto IsFloating() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the stored value is a boolean.
   *
   * \return `true` if the stored value is a boolean; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto IsBoolean() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the stored value is a file path.
   *
   * \return `true` if the stored value is a file path; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto IsFile() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the stored value is an object ID.
   *
   * \return `true` if the stored value is an object ID; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto IsObject() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the stored value is a color.
   *
   * \return `true` if the stored value is a color; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto IsColor() const noexcept -> bool;

  /**
   * \brief Returns the type of the stored value.
   *
   * \return the type of the stored value; `nothing` if there is no value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto Type() const -> Maybe<PropertyType>;

 private:
  value_type mValue;

  /**
   * \brief Indicates if the type of the value matches the specified type.
   *
   * \tparam T the type that will be checked.
   *
   * \return `true` if the property stores a value of the specified type;
   * `false` otherwise.
   *
   * \since 0.2.0
   */
  template <IsPropertyType T>
  [[nodiscard]] auto Is() const noexcept -> bool
  {
    return std::holds_alternative<T>(mValue);
  }

  /**
   * \brief Returns the property value.
   *
   * \note This function throws if there is a type mismatch.
   *
   * \tparam T the type of the value, must match the actual underlying value.
   *
   * \return the stored value.
   *
   * \since 0.2.0
   */
  template <IsPropertyType T>
  [[nodiscard]] auto As() const -> const T&
  {
    return std::get<T>(mValue);
  }

  /**
   * \brief Returns a pointer to the stored value.
   *
   * \tparam T the type of the stored value.
   *
   * \return a pointer to the stored value; a null pointer is returned if there
   * is a type mismatch.
   *
   * \since 0.2.0
   */
  template <IsPropertyType T>
  [[nodiscard]] auto TryAs() const noexcept -> const T*
  {
    return std::get_if<T>(&mValue);
  }
};

/**
 * \brief Returns the save file name associated with a property type.
 *
 * \param type the property type that will be stringified.
 *
 * \return the name of the property type used in save files.
 *
 * \since 0.2.0
 */
[[nodiscard]] auto Stringify(PropertyType type) -> QString;

}  // namespace tactile::core
