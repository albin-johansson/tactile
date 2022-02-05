#pragma once

#include <string>  // string

#include "core/attribute_value.hpp"
#include "core/components/component.hpp"
#include "tactile.hpp"

namespace tactile {

struct OpenComponentEditorEvent final
{};

struct CreateComponentDefEvent final
{
  std::string name;
};

struct RenameComponentDefEvent final
{
  component_id id{};
  std::string name;
};

struct RemoveComponentDefEvent final
{
  component_id id{};
};

struct CreateComponentAttributeEvent final
{
  component_id id{};
  std::string name;
};

struct RemoveComponentAttributeEvent final
{
  component_id id{};
  std::string name;
};

struct RenameComponentAttributeEvent final
{
  component_id id{};
  std::string previous;
  std::string updated;
};

struct DuplicateComponentAttributeEvent final
{
  component_id id{};
  std::string attribute;
};

struct SetComponentAttributeTypeEvent final
{
  component_id id{};
  std::string attribute;
  attribute_type type{};
};

struct UpdateComponentDefAttributeEvent final
{
  component_id id{};
  std::string attribute;
  attribute_value value;
};

struct AddComponentEvent final
{
  context_id context{};
  component_id component{};
};

struct RemoveComponentEvent final
{
  context_id context{};
  component_id component{};
};

struct ResetComponentValuesEvent final
{
  context_id context{};
  component_id component{};
};

struct UpdateComponentEvent final
{
  context_id context{};
  component_id component{};
  std::string attribute;
  attribute_value value;
};

}  // namespace tactile