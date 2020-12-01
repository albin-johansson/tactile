#pragma once

#include <QString>
#include <variant>  // variant, monostate

#include "maybe.hpp"

namespace tactile::core {

template <typename T>
concept property_type = std::same_as<T, QString> || std::same_as<T, int> ||
                        std::same_as<T, double> || std::same_as<T, bool>;

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
  // TODO types: string, int, float, object, file, bool, color

 public:
  using value_type = std::variant<std::monostate, QString, int, double, bool>;

  /**
   * \brief Provides values for all different possible property types.
   *
   * \since 0.2.0
   */
  enum type
  {
    string,
    integer,
    floating,
    boolean,
    file,
    color,
    object
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
  explicit property(const T& value)
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
  void set_value(const T& value)
  {
    m_value.emplace<T>(value);
  }

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
   * \brief Indicates whether or not the property holds a value.
   *
   * \return `true` if the property holds a value; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto has_value() const noexcept -> bool;

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
   * \brief Returns the type of the stored value.
   *
   * \return the type of the stored value; `std::nullopt` if there is no value.
   *
   * \since 0.2.0
   */
  [[nodiscard]] auto get_type() const -> maybe<type>;

 private:
  value_type m_value;
};

/**
 * \brief Indicates whether or not two properties are equal.
 *
 * \param lhs the left-hand side property.
 * \param rhs the right-hand side property.
 *
 * \return `true` if the properties are equal; `false` otherwise.
 *
 * \since 0.2.0
 */
[[nodiscard]] inline auto operator==(const property& lhs, const property& rhs)
    -> bool
{
  return lhs.get_type() == rhs.get_type();
}

/**
 * \brief Indicates whether or not two properties aren't equal.
 *
 * \param lhs the left-hand side property.
 * \param rhs the right-hand side property.
 *
 * \return `true` if the properties aren't equal; `false` otherwise.
 *
 * \since 0.2.0
 */
[[nodiscard]] inline auto operator!=(const property& lhs, const property& rhs)
    -> bool
{
  return !(lhs == rhs);
}

}  // namespace tactile::core
