/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "editor/ui/components/component_editor.hpp"
#include "editor/ui/components/dialogs/add_component_attr_dialog.hpp"
#include "editor/ui/components/dialogs/define_component_dialog.hpp"
#include "editor/ui/components/dialogs/rename_component_attribute_dialog.hpp"
#include "editor/ui/components/dialogs/rename_component_dialog.hpp"
#include "editor/ui/dialogs/about_dialog.hpp"
#include "editor/ui/dialogs/create_map_dialog.hpp"
#include "editor/ui/dialogs/credits_dialog.hpp"
#include "editor/ui/dialogs/map_parse_error_dialog.hpp"
#include "editor/ui/dialogs/resize_map_dialog.hpp"
#include "editor/ui/dialogs/settings_dialog.hpp"
#include "editor/ui/layers/add_layer_context_menu.hpp"
#include "editor/ui/layers/dialogs/rename_layer_dialog.hpp"
#include "editor/ui/properties/dialogs/add_property_dialog.hpp"
#include "editor/ui/properties/dialogs/change_property_type_dialog.hpp"
#include "editor/ui/properties/dialogs/rename_property_dialog.hpp"
#include "editor/ui/tilesets/dialogs/create_tileset_dialog.hpp"

namespace tactile::ui {

struct DialogState final
{
  SettingsDialog settings;
  AboutDialog    about;
  CreditsDialog  credits;

  /* Maps */
  CreateMapDialog     create_map;
  ResizeMapDialog     resize_map;
  MapParseErrorDialog map_parse_error;

  /* Layers */
  RenameLayerDialog rename_layer;

  /* Tilesets */
  CreateTilesetDialog create_tileset;

  /* Components */
  ComponentEditor                component_editor;
  DefineComponentDialog          define_component;
  AddComponentAttrDialog         add_component_attr;
  RenameComponentDialog          rename_component;
  RenameComponentAttributeDialog rename_component_attr;

  /* Properties */
  AddPropertyDialog        add_property;
  RenamePropertyDialog     rename_property;
  ChangePropertyTypeDialog change_property_type;
};

}  // namespace tactile::ui
