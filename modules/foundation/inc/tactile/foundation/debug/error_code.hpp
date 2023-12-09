// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \interface IErrorDomain
 * \brief Handles conversion of domain-specific error codes into human-readable messages.
 *
 * \details Implementations of this interface are intended to be accessed through global
 *          constants, which can then safely be referenced when creating associated
 *          `ErrorCode` instances.
 */
class IErrorDomain {
 public:
  TACTILE_INTERFACE_CLASS(IErrorDomain);

  /**
   * \brief Converts an error code to an error message.
   *
   * \param error_id the error code.
   * \return a human-readable error message, or an empty string for invalid error codes.
   */
  [[nodiscard]]
  virtual auto get_message(uint32 error_id) const noexcept -> StringView = 0;
};

/**
 * \brief Generic error code representation.
 *
 * \details All error codes identifiers are associated to an error domain. The domain
 *          can be used to convert abstract error code identifiers to error messages.
 */
class TACTILE_FOUNDATION_API ErrorCode final {
 public:
  /**
   * \brief Creates an error code.
   *
   * \param domain the associated error domain. Shouldn't (but can safely) be null.
   * \param id     the error code identifier.
   */
  constexpr ErrorCode(const IErrorDomain* domain, const uint32 id) noexcept
    : mDomain {domain},
      mId {id}
  {}

  /**
   * \brief Returns the error message associated with the error code.
   *
   * \return an error message, or `N/A` if the error domain is null.
   */
  [[nodiscard]]
  auto get_message() const noexcept -> StringView
  {
    return mDomain ? mDomain->get_message(mId) : "N/A";
  }

  /**
   * \brief Returns the associated error code identifier.
   *
   * \return an integer identifier.
   */
  [[nodiscard]]
  constexpr auto id() const noexcept -> uint32
  {
    return mId;
  }

 private:
  const IErrorDomain* mDomain {};
  uint32 mId {};
};

}  // namespace tactile