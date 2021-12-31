#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include "core/components/component.hpp"

namespace Tactile {

struct CreateComponentDefEvent final {
  std::string name;
};

struct RenameComponentDefEvent final {
  ComponentID id{};
  std::string name;
};

struct DeleteComponentDefEvent final {
  ComponentID id{};
};

struct ResetComponentEvent final {
  ComponentID id{};
};

struct CreateComponentAttributeEvent final {
  ComponentID id{};
  std::string name;
};

struct RenameComponentAttributeEvent final {
  std::string previous;
  std::string updated;
};

}  // namespace Tactile