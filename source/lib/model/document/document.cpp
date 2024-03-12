// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "document.hpp"

#include <utility>  // move

#include "core/context/context_manager.hpp"
#include "model/cmd/command_stack.hpp"
#include "model/cmd/comp/all.hpp"
#include "model/cmd/property/all.hpp"

namespace tactile {

void Document::define_component(String name)
{
  get_history().push<cmd::DefineComponent>(get_component_index_ptr(), std::move(name));
}

void Document::undef_component(const UUID& component_id)
{
  get_history().push<cmd::UndefComponent>(this, component_id);
}

void Document::rename_component(const UUID& component_id, String name)
{
  get_history().push<cmd::RenameComponent>(get_component_index_ptr(),
                                           component_id,
                                           std::move(name));
}

void Document::add_component_attribute(const UUID& component_id, String name)
{
  get_history().push<cmd::AddComponentAttr>(this, component_id, std::move(name));
}

void Document::remove_component_attribute(const UUID& component_id, String name)
{
  get_history().push<cmd::RemoveComponentAttr>(this, component_id, std::move(name));
}

void Document::rename_component_attribute(const UUID& component_id,
                                          String old_name,
                                          String new_name)
{
  get_history().push<cmd::RenameComponentAttr>(this,
                                               component_id,
                                               std::move(old_name),
                                               std::move(new_name));
}

void Document::duplicate_component_attribute(const UUID& component_id, String name)
{
  get_history().push<cmd::DuplicateComponentAttr>(this, component_id, std::move(name));
}

void Document::set_component_attribute_type(const UUID& component_id,
                                            String name,
                                            const AttributeType type)
{
  get_history().push<cmd::SetComponentAttrType>(this,
                                                component_id,
                                                std::move(name),
                                                type);
}

void Document::update_component(const UUID& component_id, String name, Attribute value)
{
  get_history().push<cmd::UpdateComponent>(get_component_index_ptr(),
                                           component_id,
                                           std::move(name),
                                           std::move(value));
}

void Document::attach_component(const UUID& context_id, const UUID& component_id)
{
  auto context = get_contexts().get_context_ptr(context_id);
  get_history().push<cmd::AttachComponent>(get_component_index_ptr(),
                                           std::move(context),
                                           component_id);
}

void Document::detach_component(const UUID& context_id, const UUID& component_id)
{
  auto context = get_contexts().get_context_ptr(context_id);
  get_history().push<cmd::DetachComponent>(std::move(context), component_id);
}

void Document::update_attached_component(const UUID& context_id,
                                         const UUID& component_id,
                                         String name,
                                         Attribute value)
{
  auto context = get_contexts().get_context_ptr(context_id);
  get_history().push<cmd::UpdateAttachedComponent>(std::move(context),
                                                   component_id,
                                                   std::move(name),
                                                   std::move(value));
}

void Document::reset_attached_component(const UUID& context_id, const UUID& component_id)
{
  auto context = get_contexts().get_context_ptr(context_id);
  get_history().push<cmd::ResetAttachedComponent>(get_component_index_ptr(),
                                                  std::move(context),
                                                  component_id);
}

void Document::add_property(const UUID& context_id, String name, const AttributeType type)
{
  auto context = get_contexts().get_context_ptr(context_id);
  get_history().push<cmd::AddProperty>(std::move(context), std::move(name), type);
}

void Document::remove_property(const UUID& context_id, String name)
{
  auto context = get_contexts().get_context_ptr(context_id);
  get_history().push<cmd::RemoveProperty>(std::move(context), std::move(name));
}

void Document::rename_property(const UUID& context_id, String current, String updated)
{
  auto context = get_contexts().get_context_ptr(context_id);
  get_history().push<cmd::RenameProperty>(std::move(context),
                                          std::move(current),
                                          std::move(updated));
}

void Document::update_property(const UUID& context_id, String name, Attribute value)
{
  auto context = get_contexts().get_context_ptr(context_id);
  get_history().push<cmd::UpdateProperty>(std::move(context),
                                          std::move(name),
                                          std::move(value));
}

void Document::change_property_type(const UUID& context_id,
                                    String name,
                                    const AttributeType type)
{
  auto context = get_contexts().get_context_ptr(context_id);
  get_history().push<cmd::ChangePropertyType>(std::move(context), std::move(name), type);
}

auto Document::is_map() const -> bool
{
  return get_type() == DocumentType::Map;
}

auto Document::is_tileset() const -> bool
{
  return get_type() == DocumentType::Tileset;
}

}  // namespace tactile
