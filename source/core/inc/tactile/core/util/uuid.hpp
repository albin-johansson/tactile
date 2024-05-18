// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>     // size_t
#include <cstring>     // memcmp
#include <format>      // format, format_to, formattable
#include <functional>  // hash
#include <ostream>     // ostream

#include "tactile/base/container/array.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"

namespace tactile {

/**
 * Represents a universally unique identifier, or UUID.
 *
 * \details
 * UUID instances are default initialized to zero. Create new UUID values
 * via the `generate` function.
 *
 * \code{cpp}
 * const UUID uuid_a {};
 * assert(uuid_a.is_null());
 *
 * const auto uuid_b = UUID::generate();
 * assert(!uuid_b.is_null());
 * assert(uuid_a != uuid_b);
 * \endcode
 */
class UUID final
{
 public:
  /**
   * Generates a new non-zero UUID.
   *
   * \return
   * A new UUID.
   */
  [[nodiscard]]
  static auto generate() -> UUID;

  /**
   * Produces a hash of the UUID.
   *
   * \return
   * A hash value.
   */
  [[nodiscard]]
  auto hash_code() const noexcept -> usize;

  /**
   * Indicates whether the UUID is null.
   *
   * \return
   * True if the underlying array is all zeroes; false otherwise.
   */
  [[nodiscard]]
  auto is_null() const -> bool;

  [[nodiscard]] friend auto operator<(const UUID& lhs,
                                      const UUID& rhs) noexcept -> bool
  {
    return std::memcmp(lhs.mData.data(), rhs.mData.data(), sizeof lhs.mData) <
           0;
  }

  [[nodiscard]] auto operator==(const UUID&) const noexcept -> bool = default;

 private:
  Array<uint8, 16> mData {};

  friend auto to_string(const UUID& uuid) -> String;
};

inline constexpr UUID kEmptyUuid = UUID {};

/**
 * Returns a textual representation of a UUID.
 *
 * \param uuid The UUID value.
 *
 * \return
 * A string with the format `HHHH-HH-HH-HH-HHHHHH`, using hexadecimal digits.
 */
[[nodiscard]]
auto to_string(const UUID& uuid) -> String;

/**
 * Outputs a UUID value to a stream.
 *
 * \param stream The output stream.
 * \param uuid   The UUID value.
 *
 * \return
 * The output stream.
 */
auto operator<<(std::ostream& stream, const UUID& uuid) -> std::ostream&;

}  // namespace tactile

template <>
struct std::hash<tactile::UUID> final
{
  [[nodiscard]] auto operator()(const tactile::UUID& uuid) const noexcept
      -> std::size_t
  {
    return uuid.hash_code();
  }
};

template <>
struct std::formatter<tactile::UUID> final
{
  template <typename FormatParseContext>
  constexpr auto parse(FormatParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const tactile::UUID& uuid, FormatContext& ctx) const
  {
    return std::format_to(ctx.out(), "{}", tactile::to_string(uuid));
  }
};

static_assert(std::formattable<tactile::UUID, char>);
