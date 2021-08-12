#ifndef RUNE_MATH_VECTOR2_HPP
#define RUNE_MATH_VECTOR2_HPP

#include <cmath>     // lerp, sqrt
#include <concepts>  // floating_point
#include <ostream>   // ostream
#include <string>    // string, to_string

#include "almost_equal.hpp"

namespace rune {

/// \addtogroup math
/// \{

/**
 * \class basic_vector2
 *
 * \brief A two-dimensional vector with floating point coordinates.
 *
 * \tparam T the representation type, must a floating-point type.
 *
 * \see `float2`
 * \see `double2`
 * \see `almost_equal(const basic_vector2<T>&, const basic_vector2<T>&, T)`
 * \see `distance(const basic_vector2<T>&, const basic_vector2<T>&)`
 * \see `angle(const basic_vector2<T>&, const basic_vector2<T>&)`
 * \see `cross(const basic_vector2<T>&, const basic_vector2<T>&)`
 * \see `dot(const basic_vector2<T>&, const basic_vector2<T>&)`
 */
template <std::floating_point T>
class basic_vector2 final
{
 public:
  using value_type = T;               ///< The type of the coordinates.
  using vector_type = basic_vector2;  ///< The type of the vector itself.

  value_type x{};  ///< The x-coordinate.
  value_type y{};  ///< The y-coordinate.

  /// \name Construction
  /// \{

  /**
   * \brief Creates a zero vector.
   */
  constexpr basic_vector2() noexcept = default;

  /**
   * \brief Creates a vector with the specified components.
   *
   * \param x the x-coordinate of the vector.
   * \param y the y-coordinate of the vector.
   */
  constexpr basic_vector2(const value_type x, const value_type y) noexcept : x{x}, y{y}
  {}

  /// \} End of construction

  /// \name Mutators
  /// \{

  /**
   * \brief Turns the vector into a zero vector.
   */
  constexpr void reset() noexcept
  {
    x = 0;
    y = 0;
  }

  /**
   * \brief Negates the coordinates of the vector.
   *
   * \details Negating a vector with components (20, 30) results in (-20, -30).
   */
  constexpr void negate() noexcept
  {
    x = -x;
    y = -y;
  }

  /**
   * \brief Normalizes the vector, i.e. the vector will be made a unit vector.
   *
   * \note If the vector is the zero vector, then this function has no effect.
   */
  constexpr void normalize()
  {
    const auto currentMagnitude = magnitude();
    if (currentMagnitude != 0) [[likely]]
    {
      x /= currentMagnitude;
      y /= currentMagnitude;
    }
  }

  /**
   * \brief Scales the coordinates of the vector by the specified factor.
   *
   * \param factor the scale factor, may be negative.
   */
  constexpr void scale(const value_type factor) noexcept
  {
    x *= factor;
    y *= factor;
  }

  /**
   * \brief Linearly interpolates the vector with the supplied target vector.
   *
   * \param target the target vector of the interpolation.
   * \param alpha the interpolation alpha, in the interval [0, 1].
   */
  constexpr void lerp(const vector_type target, const value_type alpha) noexcept
  {
    x = std::lerp(x, target.x, alpha);
    y = std::lerp(y, target.y, alpha);
  }

  /**
   * \brief Makes the vector "look" at the supplied point.
   *
   * \note A negative length value will turn the vector into the zero vector.
   *
   * \param target the point that the vector will look at.
   * \param length the length of the vector after the operation.
   *
   * \since 0.1.0
   */
  constexpr void look_at(const vector_type target, const value_type length)
  {
    if (length >= 0) [[likely]]
    {
      x = target.x - x;
      y = target.y - y;
      normalize();
      scale(length);
    }
    else
    {
      reset();
    }
  }

  /**
   * \brief Makes the vector "look" at the supplied point.
   *
   * \note The vector will maintain its current magnitude.
   *
   * \param target the point that the vector will look at.
   */
  constexpr void look_at(const vector_type target)
  {
    look_at(target, magnitude());
  }

  /**
   * \brief Sets the magnitude of the vector.
   *
   * \note If the supplied magnitude is negative, the vector becomes the zero vector.
   *
   * \param length the new magnitude of the vector.
   */
  void set_magnitude(const T length)
  {
    if (length > 0) [[likely]]
    {
      const auto previous = magnitude();
      if (previous != 0 && previous != length)
      {
        scale(length / previous);
      }
    }
    else
    {
      reset();
    }
  }

  /// \} End of mutators

  /// \name Queries
  /// \{

  /// Returns the magnitude, i.e. length, of the vector.

  /**
   * \brief Returns the magnitude (length) of the vector.
   *
   * \return the magnitude of the vector.
   */
  [[nodiscard]] auto magnitude() const -> value_type
  {
    return std::sqrt((x * x) + (y * y));
  }

  /**
   * \brief Returns the squared magnitude of this vector.
   *
   * \details This function can be useful when comparing vectors. It avoids a relatively
   * expensive square root computation.
   *
   * \return the squared magnitude of this vector.
   */
  [[nodiscard]] constexpr auto magnitude2() const noexcept -> value_type
  {
    return (x * x) + (y * y);
  }

  /**
   * \brief Indicates whether or not the vector is a unit vector.
   *
   * \details A unit vector is a vector with length `1`.
   *
   * \return `true` if the vector is a unit vector; `false` otherwise.
   */
  [[nodiscard]] auto is_unit() const -> bool
  {
    return almost_equal(magnitude(), value_type{1});
  }

  /**
   * \brief Indicates whether or not the vector is a zero vector.
   *
   * \details A vector is a zero vector if both of its coordinates are zero.
   *
   * \return `true` if the vector is a zero vector; `false` otherwise.
   */
  [[nodiscard]] auto is_zero() const -> bool
  {
    return almost_equal(x, value_type{0}) && almost_equal(y, value_type{0});
  }

  /// \} End of queries

  /// \name Serialization
  /// \{

  /**
   * \brief Serializes the vector.
   *
   * \param archive the serialization archive that will be used.
   */
  void serialize(auto& archive)
  {
    archive(x, y);
  }

  /// \} End of serialization

  /// \name Comparisons
  /// \{

  /**
   * \brief Indicates whether or not two vectors are *exactly* equal.
   *
   * \return `true` if the vectors are exactly equal; `false` otherwise.
   *
   * \see `almost_equal(const basic_vector2<T>&, const basic_vector2<T>&, T)`
   */
  [[nodiscard]] constexpr bool operator==(const vector_type&) const noexcept = default;

  /// \} End of comparisons
};

/// A two-dimensional vector using `float` precision.
using float2 = basic_vector2<float>;

/// A two-dimensional vector using `double` precision.
using double2 = basic_vector2<double>;

/// \name Vector operators
/// \{

template <std::floating_point T>
constexpr void operator+=(basic_vector2<T>& a, const basic_vector2<T>& b) noexcept
{
  a.x += b.x;
  a.y += b.y;
}

template <std::floating_point T>
[[nodiscard]] constexpr auto operator+(const basic_vector2<T>& lhs,
                                       const basic_vector2<T>& rhs) noexcept
    -> basic_vector2<T>
{
  return basic_vector2{lhs.x + rhs.x, lhs.y + rhs.y};
}

template <std::floating_point T>
constexpr void operator-=(basic_vector2<T>& a, const basic_vector2<T>& b) noexcept
{
  a.x -= b.x;
  a.y -= b.y;
}

template <std::floating_point T>
[[nodiscard]] constexpr auto operator-(const basic_vector2<T>& lhs,
                                       const basic_vector2<T>& rhs) noexcept
    -> basic_vector2<T>
{
  return basic_vector2{lhs.x - rhs.x, lhs.y - rhs.y};
}

/**
 * \brief Returns the dot product of two vectors.
 *
 * \note The dot product is commutative, which means that the order of
 * the operands doesn't matter.
 *
 * \param a the first vector.
 * \param b the second vector.
 *
 * \return the dot product of the two vectors.
 */
template <std::floating_point T>
[[nodiscard]] constexpr auto operator*(const basic_vector2<T>& a,
                                       const basic_vector2<T>& b) noexcept -> T
{
  return (a.x * b.x) + (a.y * b.y);
}

template <std::floating_point T, typename U>
constexpr void operator*=(basic_vector2<T>& vector, const U factor) noexcept
{
  vector.scale(factor);
}

template <std::floating_point T, typename U>
[[nodiscard]] constexpr auto operator*(const basic_vector2<T>& vector,
                                       const U factor) noexcept -> basic_vector2<T>
{
  return basic_vector2{vector.x * factor, vector.y * factor};
}

template <std::floating_point T>
[[nodiscard]] constexpr auto operator*(const T factor,
                                       const basic_vector2<T>& vector) noexcept
    -> basic_vector2<T>
{
  return vector * factor;
}

/// \} End of vector operators

/// \name Vector functions
/// \{

/**
 * \brief Indicates whether or not two vectors are *almost* equal.
 *
 * \param a the first vector.
 * \param b the second vector.
 * \param epsilon the epsilon value that will be used.
 *
 * \return `true` if the vectors are almost equal; `false` otherwise.
 */
template <std::floating_point T>
[[nodiscard]] auto almost_equal(const basic_vector2<T>& a,
                                const basic_vector2<T>& b,
                                const T epsilon = default_epsilon) -> bool
{
  return almost_equal(a.x, b.x, epsilon) && almost_equal(a.y, b.y, epsilon);
}

/**
 * \brief Returns the distance between two vectors.
 *
 * \details The vectors are treated as points in the plane by this function.
 *
 * \param a the first vector.
 * \param b the second vector.
 *
 * \return the distance between the two points.
 */
template <std::floating_point T>
[[nodiscard]] auto distance(const basic_vector2<T>& a, const basic_vector2<T>& b) -> T
{
  const auto dx = b.x - a.x;
  const auto dy = b.y - a.y;
  return std::sqrt(dx * dx + dy * dy);
}

/**
 * \brief Returns the cross product between two vectors.
 *
 * \param a the first vector.
 * \param b the second vector.
 *
 * \return the cross product of the vectors.
 */
template <std::floating_point T>
[[nodiscard]] constexpr auto cross(const basic_vector2<T>& a,
                                   const basic_vector2<T>& b) noexcept -> T
{
  return a.x * b.y - a.y * b.x;
}

/// \copydoc operator*(const basic_vector2<T>&, const basic_vector2<T>&)
template <std::floating_point T>
[[nodiscard]] constexpr auto dot(const basic_vector2<T>& a,
                                 const basic_vector2<T>& b) noexcept -> T
{
  return a * b;
}

/**
 * \brief Returns the angle between two vectors.
 *
 * \note This function returns zero if any of the supplied vectors are zero vectors.
 *
 * \param a the first vector.
 * \param b the second vector.
 *
 * \return the angle between the two vectors.
 */
template <std::floating_point T>
[[nodiscard]] auto angle(const basic_vector2<T>& a, const basic_vector2<T>& b) -> T
{
  if (a.is_zero() || b.is_zero() || a == b)
  {
    return 0;
  }

  const auto mag1 = a.magnitude();
  const auto mag2 = b.magnitude();

  const auto cos = (a * b) / mag1 / mag2;
  const auto sin = cross(a, b) / mag1 / mag2;

  if (const auto angle = std::acos(cos); sin < 0)
  {
    return -angle;
  }
  else
  {
    return angle;
  }
}

/// \} End of vector functions

template <std::floating_point T>
[[nodiscard]] auto to_string(const basic_vector2<T> vec) -> std::string
{
  return "vector2{x: " + std::to_string(vec.x) + ", y: " + std::to_string(vec.y) + "}";
}

template <std::floating_point T>
auto operator<<(std::ostream& stream, const basic_vector2<T> vec) -> std::ostream&
{
  return stream << to_string(vec);
}

/// \} End of group math

}  // namespace rune

#endif  // RUNE_MATH_VECTOR2_HPP
