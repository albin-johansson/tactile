// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string_view>  // string_view

#include "tactile/base/prelude.hpp"

namespace tactile::core {

/** RAII type for changing the logger identifier of the current scope. */
class SetLogScope final
{
 public:
  /**
   * Changes the logger scope identifier.
   *
   * \param scope The new scope identifier.
   */
  [[nodiscard]]
  explicit SetLogScope(std::string_view scope) noexcept;

  /** Restores the previous logger scope identifier. */
  ~SetLogScope() noexcept;

  TACTILE_DELETE_COPY(SetLogScope);
  TACTILE_DEFAULT_MOVE(SetLogScope);

 private:
  std::string_view mPrevScope;
};

}  // namespace tactile::core

#define TACTILE_SET_LOG_SCOPE(Name)              \
  const ::tactile::SetLogScope tactile_log_scope \
  {                                              \
    (Name)                                       \
  }
