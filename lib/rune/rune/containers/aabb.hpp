#ifndef RUNE_AABB_HPP_
#define RUNE_AABB_HPP_

#include <algorithm>  // min, max
#include <cassert>    // assert
#include <concepts>   // floating_point

#include "../math/vector2.hpp"

namespace rune {

/// \addtogroup containers
/// \{

/**
 * \struct basic_aabb
 *
 * \brief Represents an axis-aligned bounding box (AABB).
 *
 * \details An AABB is really just a fancy rectangle, used to provide a rough
 * approximation of the shape of game objects in order to speed up collision detection in
 * combination with AABB trees.
 *
 * \see `aabb_tree`
 * \see `make_aabb()`
 */
template <std::floating_point T>
struct basic_aabb final
{
  using precision_type = T;  ///< The type used as coordinates in the vectors, etc.
  using vector_type = basic_vector2<precision_type>;  ///< The associated vector type.

  vector_type min;  ///< The lower bound point, i.e. the upper left corner of the AABB.
  vector_type max;  ///< The upper bound point, i.e. the lower right corner of the AABB.
  precision_type area{};  ///< The area heuristic of the AABB.

  /**
   * \brief Indicates whether or not the AABB contains another AABB.
   *
   * \details The supplied AABB is still considered to be contained within the
   * invoked AABB if the borders of the "inner" AABB are overlapping the borders
   * of the "outer" AABB.
   *
   * \param other the AABB that will be checked.
   *
   * \return `true` if the AABB contains the supplied AABB; `false` otherwise.
   */
  [[nodiscard]] constexpr auto contains(const basic_aabb& other) const noexcept -> bool
  {
    return (other.min.x >= min.x) && (other.min.y >= min.y) && (other.max.x <= max.x) &&
           (other.max.y <= max.y);
  }

  /**
   * \brief Returns the size (width and height) of the AABB, represented as a vector.
   *
   * \return the size of the AABB.
   */
  [[nodiscard]] constexpr auto size() const noexcept -> vector_type
  {
    return max - min;
  }
};

/**
 * \brief Serializes an AABB.
 *
 * \param archive the serialization archive that will be used.
 *
 * \param aabb the AABB that will be serialized.
 */
template <std::floating_point T>
void serialize(auto& archive, basic_aabb<T>& aabb)
{
  archive(aabb.min, aabb.max, aabb.area);
}

/// \name AABB operators
/// \{

/**
 * \brief Indicates whether or not two AABBs are equal.
 *
 * \param a the first AABB.
 * \param b the second AABB.
 *
 * \return `true` if the AABBs are *exactly* equal; `false` otherwise.
 */
template <std::floating_point T>
[[nodiscard]] constexpr auto operator==(const basic_aabb<T>& a,
                                        const basic_aabb<T>& b) noexcept -> bool
{
  return a.min == b.min && a.max == b.max;
}

/**
 * \brief Indicates whether or not two AABBs aren't equal.
 *
 * \param a the first AABB.
 * \param b the second AABB.
 *
 * \return `true` if the AABBs aren't equal; `false` otherwise.
 */
template <std::floating_point T>
[[nodiscard]] constexpr auto operator!=(const basic_aabb<T>& a,
                                        const basic_aabb<T>& b) noexcept -> bool
{
  return !(a == b);
}

/// \} End of AABB operators

/// \name AABB functions
/// \{

/**
 * \brief Computes the area heuristic of an AABB.
 *
 * \param aabb the AABB for which the area heuristic will be calculated.
 *
 * \return the area heuristic of the AABB.
 */
template <std::floating_point T>
[[nodiscard]] constexpr auto compute_area(const basic_aabb<T>& aabb) noexcept -> T
{
  T sum{0};

  for (auto a = 0; a < 2; ++a)
  {
    T product{1};

    for (auto b = 0; b < 2; ++b)
    {
      if (a == b)
      {
        continue;
      }

      if (b == 0)
      {
        product *= (aabb.max.x - aabb.min.x);
      }
      else if (b == 1)
      {
        product *= (aabb.max.y - aabb.min.y);
      }
    }

    sum += product;
  }

  return T{2} * sum;
}

/**
 * \brief Creates an AABB.
 *
 * \pre `lower` must have coordinates smaller than those of `upper`.
 *
 * \details Use of this function is the recommended way to create AABBs, since this
 * function will compute the area heuristic for you.
 *
 * \param lower the lower bound point, i.e. the upper left corner of the AABB.
 * \param upper the upper bound point, i.e. the lower right corner of the AABB.
 *
 * \return the created AABB.
 */
template <std::floating_point T>
[[nodiscard]] constexpr auto make_aabb(const basic_vector2<T>& lower,
                                       const basic_vector2<T>& upper) noexcept
    -> basic_aabb<T>
{
  assert(lower.x < upper.x);
  assert(lower.y < upper.y);

  basic_aabb<T> aabb;

  aabb.min = lower;
  aabb.max = upper;
  aabb.area = compute_area(aabb);

  return aabb;
}

/**
 * \brief Returns the union of two AABBs.
 *
 * \param a the first AABB.
 * \param b the second AABB.
 *
 * \return an AABB that corresponds to the union of the two AABBs.
 */
template <std::floating_point T>
[[nodiscard]] constexpr auto merge(const basic_aabb<T>& a, const basic_aabb<T>& b)
    -> basic_aabb<T>
{
  basic_vector2<T> lower;
  basic_vector2<T> upper;

  lower.x = std::min(a.min.x, b.min.x);
  lower.y = std::min(a.min.y, b.min.y);

  upper.x = std::max(a.max.x, b.max.x);
  upper.y = std::max(a.max.y, b.max.y);

  return make_aabb(lower, upper);
}

/**
 * \brief Indicates whether or not two AABBs are overlapping each other.
 *
 * \param a the first AABB.
 * \param b the second AABB.
 * \param touchIsOverlap `true` if two "touching" AABBs should be considered to overlap;
 * `false` otherwise.
 *
 * \return `true` if the AABBs overlap; `false` otherwise.
 */
template <std::floating_point T>
[[nodiscard]] constexpr auto overlaps(const basic_aabb<T>& a,
                                      const basic_aabb<T>& b,
                                      const bool touchIsOverlap) noexcept -> bool
{
  if (touchIsOverlap)
  {
    return !(b.max.x < a.min.x || b.min.x > a.max.x || b.max.y < a.min.y ||
             b.min.y > a.max.y);
  }
  else
  {
    return !(b.max.x <= a.min.x || b.min.x >= a.max.x || b.max.y <= a.min.y ||
             b.min.y >= a.max.y);
  }
}

/**
 * \brief Fattens an AABB by the specified factor.
 *
 * \details The supplied AABB will be enlarged by having *each* coordinate (i.e. X- and
 * Y-coordinates of both lower and upper bounds) widened by the specified percentage, e.g.
 * an AABB with width 100 that is fattened with `percentage` specified as `0.05`, will end
 * up with having width 110.
 *
 * \param aabb the AABB that will be fattened.
 * \param percentage the percentage of the current size that each coordinate will be
 * enlarged by.
 */
template <std::floating_point T>
void fatten(basic_aabb<T>& aabb, const T percentage) noexcept
{
  const auto size = aabb.size();
  const auto dx = percentage * size.x;
  const auto dy = percentage * size.y;

  aabb.min.x -= dx;
  aabb.min.y -= dy;

  aabb.max.x += dx;
  aabb.max.y += dy;

  aabb.area = compute_area(aabb);
}

/// \} End of AABB functions

/// \} End of group containers

}  // namespace rune

#endif  // RUNE_AABB_HPP_
