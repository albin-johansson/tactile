/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "model.hpp"

#include "model/document.hpp"

namespace tactile {
namespace {

inline Model gModel;

}  // namespace

Model::Model()
{
  auto& document_context = add<CDocumentContext>();
  document_context.active_document = kNullEntity;
}

auto Model::create_entity() -> Entity
{
  return mRegistry.create();
}

void Model::destroy(const Entity entity)
{
  if (entity != kNullEntity) {
    TACTILE_ASSERT(mRegistry.valid(entity));
    mRegistry.destroy(entity);
  }
}

void Model::set_enabled(const Entity entity, const bool enabled)
{
  TACTILE_ASSERT(mRegistry.valid(entity));
  if (enabled) {
    mRegistry.remove<DisabledTag>(entity);
  }
  else {
    mRegistry.emplace_or_replace<DisabledTag>(entity);
  }
}

auto Model::is_enabled(const Entity entity) const -> bool
{
  TACTILE_ASSERT(mRegistry.valid(entity));
  return !mRegistry.all_of<DisabledTag>(entity);
}

auto get_model() -> Model&
{
  return gModel;
}

}  // namespace tactile
