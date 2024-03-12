// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/** RAII type for changing the logger identifier of the current scope. */
class SetLogScope final {
 public:
  /**
   * Changes the logger scope identifier.
   *
   * \param scope The new scope identifier.
   */
  [[nodiscard]]
  explicit SetLogScope(StringView scope) noexcept;

  /** Restores the previous logger scope identifier. */
  ~SetLogScope() noexcept;

  TACTILE_DELETE_COPY(SetLogScope);
  TACTILE_DEFAULT_MOVE(SetLogScope);

 private:
  StringView mPrevScope;
};

}  // namespace tactile
