// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <entt/signal/dispatcher.hpp>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

#include "common/util/fmt.hpp"
#include "handlers/event_handlers.hpp"
#include "model/document/document.hpp"
#include "model/event/component_events.hpp"
#include "model/model.hpp"
#include "runtime/app_context.hpp"
#include "ui/dock/comp/component_editor.hpp"

namespace tactile {
namespace {

void on_open_component_editor(const OpenComponentEditorEvent&)
{
  spdlog::trace("OpenComponentEditorEvent");
  ui::open_component_editor_dialog(get_model());
}

void on_define_component(const DefineComponentEvent& event)
{
  spdlog::trace("DefineComponentEvent(name: {})", event.name);
  if (auto* document = get_model().active_document()) {
    document->define_component(event.name);
  }
}

void on_undef_component(const UndefComponentEvent& event)
{
  spdlog::trace("UndefComponentEvent(component_id: {})", event.component_id);
  if (auto* document = get_model().active_document()) {
    document->undef_component(event.component_id);
  }
}

void on_rename_component(const RenameComponentEvent& event)
{
  spdlog::trace("RenameComponentEvent(component_id: {}, name: {})",
                event.component_id,
                event.name);
  if (auto* document = get_model().active_document()) {
    document->rename_component(event.component_id, event.name);
  }
}

void on_update_component(const UpdateComponentEvent& event)
{
  spdlog::trace("UpdateComponentEvent(component_id: {}, attr_name: {})",
                event.component_id,
                event.attr_name);
  if (auto* document = get_model().active_document()) {
    document->update_component(event.component_id, event.attr_name, event.value);
  }
}

void on_attach_component(const AttachComponentEvent& event)
{
  spdlog::trace("AttachComponentEvent(context_id: {}, component_id: {})",
                event.context_id,
                event.component_id);
  if (auto* document = get_model().active_document()) {
    document->attach_component(event.context_id, event.component_id);
  }
}

void on_detach_component(const DetachComponentEvent& event)
{
  spdlog::trace("DetachComponentEvent(context_id: {}, component_id: {})",
                event.context_id,
                event.component_id);
  if (auto* document = get_model().active_document()) {
    document->detach_component(event.context_id, event.component_id);
  }
}

void on_update_attached_component(const UpdateAttachedComponentEvent& event)
{
  spdlog::trace(
      "UpdateAttachedComponentEvent(context_id: {}, component_id: {}, attr_name: {})",
      event.context_id,
      event.component_id,
      event.attr_name);
  if (auto* document = get_model().active_document()) {
    document->update_attached_component(event.context_id,
                                        event.component_id,
                                        event.attr_name,
                                        event.value);
  }
}

void on_reset_attached_component(const ResetAttachedComponentEvent& event)
{
  spdlog::trace("ResetAttachedComponentEvent(context_id: {}, component_id: {})",
                event.context_id,
                event.component_id);
  if (auto* document = get_model().active_document()) {
    document->reset_attached_component(event.context_id, event.component_id);
  }
}

void on_add_component_attr(const AddComponentAttrEvent& event)
{
  spdlog::trace("AddComponentAttrEvent(component_id: {}, attr_name: {})",
                event.component_id,
                event.attr_name);
  if (auto* document = get_model().active_document()) {
    document->add_component_attribute(event.component_id, event.attr_name);
  }
}

void on_remove_component_attr(const RemoveComponentAttrEvent& event)
{
  spdlog::trace("RemoveComponentAttrEvent(component_id: {}, attr_name: {})",
                event.component_id,
                event.attr_name);
  if (auto* document = get_model().active_document()) {
    document->remove_component_attribute(event.component_id, event.attr_name);
  }
}

void on_rename_component_attr(const RenameComponentAttrEvent& event)
{
  spdlog::trace(
      "RenameComponentAttrEvent(component_id: {}, current_name: {}, updated_name: {})",
      event.component_id,
      event.current_name,
      event.updated_name);
  if (auto* document = get_model().active_document()) {
    document->rename_component_attribute(event.component_id,
                                         event.current_name,
                                         event.updated_name);
  }
}

void on_duplicate_component_attr(const DuplicateComponentAttrEvent& event)
{
  spdlog::trace("DuplicateComponentAttrEvent(component_id: {}, attr_name: {})",
                event.component_id,
                event.attr_name);
  if (auto* document = get_model().active_document()) {
    document->duplicate_component_attribute(event.component_id, event.attr_name);
  }
}

void on_set_component_attr_type(const SetComponentAttrTypeEvent& event)
{
  spdlog::trace("SetComponentAttrTypeEvent(component_id: {}, attr_name: {}, type: {})",
                event.component_id,
                event.attr_name,
                magic_enum::enum_name(event.type));
  if (auto* document = get_model().active_document()) {
    document->set_component_attribute_type(event.component_id,
                                           event.attr_name,
                                           event.type);
  }
}

}  // namespace

void install_component_event_handler()
{
  auto& dispatcher = get_dispatcher();

  dispatcher.sink<OpenComponentEditorEvent>().connect<&on_open_component_editor>();

  dispatcher.sink<DefineComponentEvent>().connect<&on_define_component>();
  dispatcher.sink<UndefComponentEvent>().connect<&on_undef_component>();
  dispatcher.sink<RenameComponentEvent>().connect<&on_rename_component>();
  dispatcher.sink<UpdateComponentEvent>().connect<&on_update_component>();

  // clang-format off
  dispatcher.sink<AttachComponentEvent>().connect<&on_attach_component>();
  dispatcher.sink<DetachComponentEvent>().connect<&on_detach_component>();
  dispatcher.sink<UpdateAttachedComponentEvent>().connect<&on_update_attached_component>();
  dispatcher.sink<ResetAttachedComponentEvent>().connect<&on_reset_attached_component>();
  // clang-format on

  dispatcher.sink<AddComponentAttrEvent>().connect<&on_add_component_attr>();
  dispatcher.sink<RemoveComponentAttrEvent>().connect<&on_remove_component_attr>();
  dispatcher.sink<RenameComponentAttrEvent>().connect<&on_rename_component_attr>();
  dispatcher.sink<DuplicateComponentAttrEvent>().connect<&on_duplicate_component_attr>();
  dispatcher.sink<SetComponentAttrTypeEvent>().connect<&on_set_component_attr_type>();
}

}  // namespace tactile
