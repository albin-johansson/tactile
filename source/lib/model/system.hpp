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

#include "common/type/dispatcher.hpp"
#include "common/type/ref.hpp"
#include "model/registry.hpp"

namespace tactile {

/// The abstract base class for all system implementations.
class System {
 public:
  /**
   * Creates a system.
   *
   * \warning The registry and event dispatcher must both outlive the system.
   *
   * \param model      the associated registry.
   * \param dispatcher the associated event dispatcher.
   */
  System(Registry& registry, Dispatcher& dispatcher);

  virtual ~System() noexcept = default;

  /**
   * Resets any potential temporary state stored in the system.
   *
   * \details This function is called for major context changes, such as whenever the
   *          active document changes.
   */
  virtual void reset() {}

  /// Installs event listeners to the associated events.
  virtual void install() {}

  /**
   * Updates the system.
   *
   * \details This function is called once for each engine tick. As such, it's important
   *          to be mindful of performance when overriding this function.
   */
  virtual void update() {}

  [[nodiscard]] auto get_registry() -> Registry& { return mRegistry.get(); }
  [[nodiscard]] auto get_registry() const -> const Registry& { return mRegistry.get(); }

  [[nodiscard]] auto get_dispatcher() const -> Dispatcher& { return mDispatcher.get(); }

 private:
  Ref<Registry> mRegistry;
  Ref<Dispatcher> mDispatcher;
};

}  // namespace tactile
