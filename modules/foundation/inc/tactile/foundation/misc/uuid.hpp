// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>     // size_t
#include <functional>  // hash

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/array.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents a universally unique identifier, or UUID.
 *
 * \details UUID instances are default initialized to zero. Create new UUID values
 *          via the `generate` function.
 *
 *          \code{cpp}
 *          const UUID uuid_a;
 *          assert(uuid_a.is_null());
 *
 *          const auto uuid_b = UUID::generate();
 *          assert(!uuid_b.is_null());
 *          assert(uuid_a != uuid_b);
 *          \endcode
 */
class TACTILE_FOUNDATION_API UUID final {
 public:
  /**
   * \brief Generates a new UUID.
   *
   * \return a new UUID.
   */
  [[nodiscard]]
  static auto generate() -> UUID;

  /**
   * \brief Produces a hash of the UUID.
   *
   * \return a hash value.
   */
  [[nodiscard]]
  auto hash_code() const noexcept -> usize;

  /**
   * \brief Indicates whether the UUID is null.
   *
   * \return true if the underlying array is all zeroes; false otherwise.
   */
  [[nodiscard]]
  auto is_null() const -> bool;

  [[nodiscard]] auto operator==(const UUID&) const noexcept -> bool = default;

 private:
  Array<uint8, 16> mData {};

  TACTILE_FOUNDATION_API friend auto to_string(const UUID& uuid) -> String;
};

/**
 * \brief Returns a textual representation of a UUID.
 *
 * \param uuid the UUID value.
 *
 * \return a string with the format `HHHH-HH-HH-HH-HHHHHH`, using hexadecimal digits.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto to_string(const UUID& uuid) -> String;

}  // namespace tactile

template <>
struct std::hash<tactile::UUID> final {
  [[nodiscard]] auto operator()(const tactile::UUID& uuid) const noexcept -> std::size_t
  {
    return uuid.hash_code();
  }
};