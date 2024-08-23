// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/log/set_log_scope.hpp"

#include "tactile/core/log/logger.hpp"

namespace tactile {

SetLogScope::SetLogScope(const std::string_view scope) noexcept
{
  if (auto* logger = get_default_logger()) {
    mPrevScope = logger->get_scope();
    logger->set_scope(scope);
  }
}

SetLogScope::~SetLogScope() noexcept
{
  if (auto* logger = get_default_logger()) {
    logger->set_scope(mPrevScope);
  }
}

}  // namespace tactile
