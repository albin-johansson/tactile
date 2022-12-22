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

#include "add_component_attr_dialog.hpp"

#include <entt/signal/dispatcher.hpp>

#include "core/component/component_index.hpp"
#include "core/type/string.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/component_events.hpp"
#include "model/model.hpp"

namespace tactile::ui {

AddComponentAttrDialog::AddComponentAttrDialog()
    : StringInputDialog {"Create Attribute"}
{
}

void AddComponentAttrDialog::show(const UUID& component_id)
{
  mComponentId = component_id;

  const auto& lang = get_current_language();
  set_title(lang.window.create_attribute);
  set_input_hint(lang.misc.attribute_name_hint);
  set_accept_button_label(lang.misc.create);

  StringInputDialog::show("");
}

auto AddComponentAttrDialog::validate(const DocumentModel& model, StringView input) const
    -> bool
{
  const auto& document = model.require_active_document();
  const auto* index = document.view_component_index();
  return !input.empty() && index && !index->at(mComponentId).has(input);
}

void AddComponentAttrDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddComponentAttrEvent>(mComponentId, String {current_input()});
}

}  // namespace tactile::ui