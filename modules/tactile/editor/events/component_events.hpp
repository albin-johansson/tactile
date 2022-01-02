#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include "core/components/component.hpp"

namespace Tactile {

struct CreateComponentDefEvent final
{
  std::string name;
};

struct RenameComponentDefEvent final
{
  ComponentID id{};
  std::string name;
};

struct RemoveComponentDefEvent final
{
  ComponentID id{};
};

struct CreateComponentAttributeEvent final
{
  ComponentID id{};
  std::string name;
};

struct RemoveComponentAttributeEvent final
{
  ComponentID id{};
  std::string name;
};

struct RenameComponentAttributeEvent final
{
  ComponentID id{};
  std::string previous;
  std::string updated;
};

struct SetComponentAttributeTypeEvent final
{
  ComponentID id{};
  std::string attribute;
  PropertyType type{};
};

struct UpdateComponentAttributeEvent final
{
  ComponentID id{};
  std::string attribute;
  PropertyValue value;
};

struct ResetComponentValuesEvent final
{
  ContextID context{};
  ComponentID component{};
};

}  // namespace Tactile