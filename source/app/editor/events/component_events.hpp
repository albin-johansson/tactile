#pragma once

#include <string>  // string

#include "core/attribute.hpp"
#include "core/components/component.hpp"
#include "tactile.hpp"

namespace tactile {

struct open_component_editor_event final
{};

struct create_component_def_event final
{
  std::string name;
};

struct rename_component_def_event final
{
  component_id id{};
  std::string name;
};

struct remove_component_def_event final
{
  component_id id{};
};

struct create_component_attr_event final
{
  component_id id{};
  std::string name;
};

struct remove_component_attr_event final
{
  component_id id{};
  std::string name;
};

struct rename_component_attr_event final
{
  component_id id{};
  std::string previous;
  std::string updated;
};

struct duplicate_component_attr_event final
{
  component_id id{};
  std::string attribute;
};

struct set_component_attr_type_event final
{
  component_id id{};
  std::string attribute;
  attribute_type type{};
};

struct update_component_def_attr_event final
{
  component_id id{};
  std::string attribute;
  attribute_value value;
};

struct add_component_event final
{
  context_id context{};
  component_id component{};
};

struct remove_component_event final
{
  context_id context{};
  component_id component{};
};

struct reset_component_values_event final
{
  context_id context{};
  component_id component{};
};

struct update_component_event final
{
  context_id context{};
  component_id component{};
  std::string attribute;
  attribute_value value;
};

}  // namespace tactile