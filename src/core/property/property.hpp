#pragma once

#include <QColor>     // QColor
#include <QFileInfo>  // QFileInfo
#include <QString>    // QString
#include <concepts>   // same_as
#include <variant>    // variant, monostate

#include "maybe.hpp"
#include "nenya.hpp"

namespace tactile::core {
namespace detail {
struct object_ref_t;
}

using object_ref = nenya::mirror_type<int, detail::object_ref_t>;

template <typename T>
concept property_type = std::same_as<T, QString> || std::same_as<T, int> ||
                        std::same_as<T, double> || std::same_as<T, bool> ||
                        std::same_as<T, QColor> || std::same_as<T, QFileInfo> ||
                        std::same_as<T, object_ref>;

/**
 * \class property
 *
 * \brief Represents a property associated with maps, layers and other objects.
 *
 * \since 0.2.0
 *
 * \headerfile property.hpp
 */
class property final
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
   * \brief Provides values for all different possible property types.
   *
   * \since 0.2.0
   */
  enum type
  {
    string,    ///< A string property.
    integer,   ///< An integer property.
    floating,  ///< A floating-point property.
    boolean,   ///< A boolean property.
    file,      ///< A file path property.
    color,     ///< A color property.
    object     ///< An integer ID property, that refers to a map object.
  };

  /**
   * \brief Creates a property with no value.
   *
   * \since 0.2.0
   */
  property() = default;

  /**
   * \brief Creates a property.
   *
   * \tparam T the type of the property value.
   *
   * \param value the value of the property.
   *
   * \since 0.2.0
   */
  template <property_type T>
  /*implicit*/ property(const T& value)
  {
    m_value.emplace<T>(value);
  }

  /**
   * \brief Resets the state of the property.
   *
   * \post `has_value()` will return `false` after this function is invoked.
   *
   * \since 0.2.0
   */
  void reset();

  void set_default(type t);

  /**
   * \brief Sets the value stored in the property.
   *
   * \tparam T the type of the value.
   *
   * \param value the new value of the property.
   *
   * \since 0.2.0
   */
  template <property_type T>
  void set_value(const T& value = {})
  {
    m_value.emplace<T>(value);
  }

  /**
   * \brief Indicates whether or not the property holds a value.
   *
   * \return `true` if the property holds a value; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto has_value() const noexcept -> bool;

  /**
   * \brief Returns the stored string value.
   *
   * \note This function throws if the stored value isn't a color.
   *
   * \return the stored color.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto as_string() const -> const QString&;

  /**
   * \brief Returns the stored integer value.
   *
   * \note This function throws if the stored value isn't an integer.
   *
   * \return the stored integer.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto as_integer() const -> int;

  /**
   * \brief Returns the stored float value.
   *
   * \note This function throws if the stored value isn't a float.
   *
   * \return the stored floating-point value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto as_floating() const -> double;

  /**
   * \brief Returns the stored boolean value.
   *
   * \note This function throws if the stored value isn't a boolean.
   *
   * \return the stored boolean value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto as_boolean() const -> bool;

  /**
   * \brief Returns the stored file path value.
   *
   * \note This function throws if the stored value isn't a file path.
   *
   * \return the stored file path value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto as_file() const -> const QFileInfo&;

  /**
   * \brief Returns the stored object ID value.
   *
   * \note This function throws if the stored value isn't an object ID.
   *
   * \return the stored object ID value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto as_object() const -> object_ref;

  /**
   * \brief Returns the stored color value.
   *
   * \note This function throws if the stored value isn't a color.
   *
   * \return the stored color value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto as_color() const -> const QColor&;

  [[nodiscard]] auto try_as_string() const noexcept -> const QString*
  {
    return try_as<QString>();
  }

  [[nodiscard]] auto try_as_integer() const noexcept -> const int*
  {
    return try_as<int>();
  }

  [[nodiscard]] auto try_as_floating() const noexcept -> const double*
  {
    return try_as<double>();
  }

  [[nodiscard]] auto try_as_boolean() const noexcept -> const bool*
  {
    return try_as<bool>();
  }

  [[nodiscard]] auto try_as_file() const noexcept -> const QFileInfo*
  {
    return try_as<QFileInfo>();
  }

  [[nodiscard]] auto try_as_object() const noexcept -> const object_ref*
  {
    return try_as<object_ref>();
  }

  [[nodiscard]] auto try_as_color() const noexcept -> const QColor*
  {
    return try_as<QColor>();
  }

  /**
   * \brief Indicates whether or not the stored value is a string.
   *
   * \return `true` if the stored value is a string; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto is_string() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the stored value is an integer.
   *
   * \return `true` if the stored value is an integer; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto is_integer() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the stored value is a floating-point
   * number.
   *
   * \return `true` if the stored value is a floating-point number; `false`
   * otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto is_floating() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the stored value is a boolean.
   *
   * \return `true` if the stored value is a boolean; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto is_boolean() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the stored value is a file path.
   *
   * \return `true` if the stored value is a file path; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto is_file() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the stored value is an object ID.
   *
   * \return `true` if the stored value is an object ID; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto is_object() const noexcept -> bool;

  /**
   * \brief Indicates whether or not the stored value is a color.
   *
   * \return `true` if the stored value is a color; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto is_color() const noexcept -> bool;

  /**
   * \brief Returns the type of the stored value.
   *
   * \return the type of the stored value; `std::nullopt` if there is no value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto get_type() const -> maybe<type>;

 private:
  value_type m_value;

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
  template <property_type T>
  [[nodiscard]] auto is() const noexcept -> bool
  {
    return std::holds_alternative<T>(m_value);
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
  template <property_type T>
  [[nodiscard]] auto as() const -> const T&
  {
    return std::get<T>(m_value);
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
  template <property_type T>
  [[nodiscard]] auto try_as() const noexcept -> const T*
  {
    return std::get_if<T>(&m_value);
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
[[nodiscard]] auto stringify(property::type type) -> QString;

}  // namespace tactile::core
