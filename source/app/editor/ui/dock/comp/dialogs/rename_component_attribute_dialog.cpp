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

#include "rename_component_attribute_dialog.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>

#include "core/comp/component_index.hpp"
#include "core/event/component_events.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"

namespace tactile::ui {

RenameComponentAttributeDialog::RenameComponentAttributeDialog()
    : StringInputDialog {"Rename Component Attribute"}
{
}

void RenameComponentAttributeDialog::show(String previous_name, const UUID& component_id)
{
  mComponentId = component_id;

  const auto& lang = get_current_language();
  set_title(lang.window.rename_component_attribute);
  set_accept_button_label(lang.misc.rename);
  set_input_hint(lang.misc.attribute_name_hint);

  StringInputDialog::show(std::move(previous_name));
}

void RenameComponentAttributeDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RenameComponentAttrEvent>(mComponentId.value(),
                                               previous_input(),
                                               String {current_input()});
}

auto RenameComponentAttributeDialog::validate(const DocumentModel& model,
                                              StringView input) const -> bool
{
  const auto& document = model.require_active_document();
  const auto* index = document.view_component_index();
  return !input.empty() && index && !index->at(mComponentId.value()).has(input);
}

}  // namespace tactile::ui