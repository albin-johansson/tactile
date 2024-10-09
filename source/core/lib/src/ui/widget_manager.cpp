// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/widget_manager.hpp"

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/shortcuts.hpp"

namespace tactile::core::ui {

void WidgetManager::push(const Model& model, EventDispatcher& dispatcher)
{
  const auto& language = model.get_language();
  const auto* current_doc = model.get_current_document();
  const auto* current_map_doc = dynamic_cast<const MapDocument*>(current_doc);

  mMenuBar.push(model, dispatcher);
  mDockSpace.update(language);
  mDocumentDock.push(model, dispatcher);

  if (current_map_doc != nullptr) {
    mTilesetDock.push(language, *current_map_doc, dispatcher);
    mLayerDock.update(language, *current_map_doc, dispatcher);
  }

  if (current_doc != nullptr) {
    mPropertyDock.push(language, *current_doc, dispatcher);
    mComponentDock.push(language, *current_doc, dispatcher);
    mLogDock.push(model, dispatcher);
  }

  mNewMapDialog.push(model, dispatcher);
  mNewTilesetDialog.push(model, dispatcher);
  mNewPropertyDialog.push(model, dispatcher);
  mRenamePropertyDialog.push(model, dispatcher);
  m_godot_export_dialog.push(model, dispatcher);

  push_global_shortcuts(model, dispatcher);
}

auto WidgetManager::get_dock_space() -> DockSpace&
{
  return mDockSpace;
}

auto WidgetManager::get_new_map_dialog() -> NewMapDialog&
{
  return mNewMapDialog;
}

auto WidgetManager::get_godot_export_dialog() -> GodotExportDialog&
{
  return m_godot_export_dialog;
}

auto WidgetManager::get_new_tileset_dialog() -> NewTilesetDialog&
{
  return mNewTilesetDialog;
}

auto WidgetManager::get_new_property_dialog() -> NewPropertyDialog&
{
  return mNewPropertyDialog;
}

auto WidgetManager::get_rename_property_dialog() -> RenamePropertyDialog&
{
  return mRenamePropertyDialog;
}

}  // namespace tactile::core::ui
