/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "tactile/core/common/assert.hpp"

namespace tactile {

/**
 * Utility class used to locate various services.
 *
 * \note The lifetime of services is not managed by the locator itself.
 *
 * \tparam Service the type of the service.
 */
template <typename Service>
class ServiceLocator final {
 public:
  /// Resets the associated service.
  static void reset(Service* service = nullptr) noexcept { mService = service; }

  /// Returns the bound service (use this only if you're certain that the service exists).
  [[nodiscard]] static auto get() noexcept -> Service&
  {
    TACTILE_ASSERT_MSG(has_value(), "The requested service was not initialized");
    return *mService;
  }

  /// Returns the bound service or a null pointer if it doesn't exist.
  [[nodiscard]] static auto try_get() noexcept -> Service* { return mService; }

  /// Indicates whether a service has been bound for this interface.
  [[nodiscard]] static auto has_value() noexcept -> bool { return mService != nullptr; }

 private:
  inline static Service* mService {nullptr};
};

}  // namespace tactile
