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

#include "model.hpp"

#include "model/model_impl.hpp"

namespace tactile {

DocumentModel::DocumentModel()
    : mImpl {std::make_unique<Impl>(this)}
{
}

DocumentModel::~DocumentModel() noexcept = default;

void DocumentModel::update()
{
  mImpl->update();
}

void DocumentModel::each(const VisitorFunc& func) const
{
  mImpl->each(func);
}

auto DocumentModel::add_map(const Int2& tile_size, const usize rows, const usize columns)
    -> UUID
{
  return mImpl->add_map(tile_size, rows, columns);
}

auto DocumentModel::add_tileset(const TilesetInfo& info) -> UUID
{
  return mImpl->add_tileset(info);
}

void DocumentModel::remove_tileset(const UUID& id)
{
  mImpl->remove_tileset(id);
}

auto DocumentModel::restore_tileset(const TileID first_tile_id, const TilesetInfo& info)
    -> UUID
{
  return mImpl->restore_tileset(first_tile_id, info);
}

void DocumentModel::select_document(const UUID& id)
{
  mImpl->select_document(id);
}

void DocumentModel::open_document(const UUID& id)
{
  mImpl->open_document(id);
}

void DocumentModel::close_document(const UUID& id)
{
  mImpl->close_document(id);
}

auto DocumentModel::has_document(const UUID& id) const -> bool
{
  return mImpl->has_document(id);
}

auto DocumentModel::has_document_with_path(const Path& path) const -> bool
{
  return mImpl->has_document_with_path(path);
}

auto DocumentModel::get_id_for_path(const Path& path) const -> UUID
{
  return mImpl->get_id_for_path(path);
}

auto DocumentModel::get_document(const UUID& id) -> Shared<Document>
{
  return mImpl->get_document(id);
}

auto DocumentModel::active_document_id() const -> Maybe<UUID>
{
  return mImpl->active_document_id();
}

auto DocumentModel::has_active_document() const -> bool
{
  return mImpl->has_active_document();
}

auto DocumentModel::is_map_active() const -> bool
{
  return mImpl->is_map_active();
}

auto DocumentModel::is_tileset_active() const -> bool
{
  return mImpl->is_tileset_active();
}

auto DocumentModel::active_document() -> Document*
{
  return mImpl->active_document();
}

auto DocumentModel::active_document() const -> const Document*
{
  return mImpl->active_document();
}

auto DocumentModel::active_map() -> MapDocument*
{
  return mImpl->active_map();
}

auto DocumentModel::active_map() const -> const MapDocument*
{
  return mImpl->active_map();
}

auto DocumentModel::active_tileset() -> TilesetDocument*
{
  return mImpl->active_tileset();
}

auto DocumentModel::active_tileset() const -> const TilesetDocument*
{
  return mImpl->active_tileset();
}

auto DocumentModel::require_active_document() const -> const Document&
{
  return mImpl->require_active_document();
}

auto DocumentModel::require_active_map() -> MapDocument&
{
  return mImpl->require_active_map();
}

auto DocumentModel::require_active_map() const -> const MapDocument&
{
  return mImpl->require_active_map();
}

auto DocumentModel::require_active_tileset() -> TilesetDocument&
{
  return mImpl->require_active_tileset();
}

auto DocumentModel::require_active_tileset() const -> const TilesetDocument&
{
  return mImpl->require_active_tileset();
}

void DocumentModel::set_command_capacity(const usize capacity)
{
  mImpl->set_command_capacity(capacity);
}

auto DocumentModel::is_open(const UUID& id) const -> bool
{
  return mImpl->is_open(id);
}

auto DocumentModel::is_map(const UUID& id) const -> bool
{
  return mImpl->is_map(id);
}

auto DocumentModel::is_tileset(const UUID& id) const -> bool
{
  return mImpl->is_tileset(id);
}

auto DocumentModel::get_map(const UUID& id) -> Shared<MapDocument>
{
  return mImpl->get_map(id);
}

auto DocumentModel::get_tileset(const UUID& id) -> Shared<TilesetDocument>
{
  return mImpl->get_tileset(id);
}

auto DocumentModel::view_document(const UUID& id) const -> const Document&
{
  return mImpl->view_document(id);
}

auto DocumentModel::view_map(const UUID& id) const -> const MapDocument&
{
  return mImpl->view_map(id);
}

auto DocumentModel::view_tileset(const UUID& id) const -> const TilesetDocument&
{
  return mImpl->view_tileset(id);
}

void DocumentModel::register_map(Shared<MapDocument> document)
{
  mImpl->register_map(std::move(document));
}

void DocumentModel::register_tileset(Shared<TilesetDocument> document)
{
  mImpl->register_tileset(std::move(document));
}

auto DocumentModel::unregister_map(const UUID& id) -> Shared<MapDocument>
{
  return mImpl->unregister_map(id);
}

auto DocumentModel::unregister_tileset(const UUID& id) -> Shared<TilesetDocument>
{
  return mImpl->unregister_tileset(id);
}

}  // namespace tactile
