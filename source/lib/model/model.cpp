// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

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

auto DocumentModel::create_map_document(const Int2& tile_size, const TileExtent extent)
    -> UUID
{
  return mImpl->add_map(tile_size, extent);
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
  mImpl->documents().select_document(id);
}

void DocumentModel::open_document(const UUID& id)
{
  mImpl->documents().open_document(id);
}

void DocumentModel::close_document(const UUID& id)
{
  mImpl->documents().close_document(id);
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

auto DocumentModel::get_document_ptr(const UUID& id) -> Shared<Document>
{
  return mImpl->documents().get_document_ptr(id);
}

auto DocumentModel::get_active_document_id() const -> Maybe<UUID>
{
  return mImpl->documents().current_document_id();
}

auto DocumentModel::has_active_document() const -> bool
{
  return mImpl->documents().current_document_id().has_value();
}

auto DocumentModel::is_map_active() const -> bool
{
  return mImpl->documents().is_map_active();
}

auto DocumentModel::is_tileset_active() const -> bool
{
  return mImpl->documents().is_tileset_active();
}

auto DocumentModel::active_document() -> Document*
{
  return mImpl->documents().current_document();
}

auto DocumentModel::active_document() const -> const Document*
{
  return mImpl->documents().current_document();
}

auto DocumentModel::active_map_document() -> MapDocument*
{
  return mImpl->documents().current_map_document();
}

auto DocumentModel::active_map_document() const -> const MapDocument*
{
  return mImpl->documents().current_map_document();
}

auto DocumentModel::active_tileset_document() -> TilesetDocument*
{
  return mImpl->documents().current_tileset_document();
}

auto DocumentModel::active_tileset_document() const -> const TilesetDocument*
{
  return mImpl->documents().current_tileset_document();
}

auto DocumentModel::require_active_document() const -> const Document&
{
  return mImpl->require_active_document();
}

auto DocumentModel::require_active_map_document() -> MapDocument&
{
  return mImpl->require_active_map();
}

auto DocumentModel::require_active_map_document() const -> const MapDocument&
{
  return mImpl->require_active_map();
}

auto DocumentModel::require_active_tileset_document() -> TilesetDocument&
{
  return mImpl->require_active_tileset();
}

auto DocumentModel::require_active_tileset_document() const -> const TilesetDocument&
{
  return mImpl->require_active_tileset();
}

void DocumentModel::set_command_capacity(const usize capacity)
{
  mImpl->set_command_capacity(capacity);
}

auto DocumentModel::is_open(const UUID& id) const -> bool
{
  return mImpl->documents().is_document_open(id);
}

auto DocumentModel::is_map(const UUID& id) const -> bool
{
  return mImpl->documents().is_map(id);
}

auto DocumentModel::is_tileset(const UUID& id) const -> bool
{
  return mImpl->documents().is_tileset(id);
}

auto DocumentModel::get_map_document_ptr(const UUID& id) -> Shared<MapDocument>
{
  return mImpl->documents().get_map_document_ptr(id);
}

auto DocumentModel::get_tileset_document_ptr(const UUID& id) -> Shared<TilesetDocument>
{
  return mImpl->documents().get_tileset_document_ptr(id);
}

auto DocumentModel::get_document(const UUID& id) const -> const Document&
{
  return mImpl->documents().get_document(id);
}

auto DocumentModel::get_map_document(const UUID& id) const -> const MapDocument&
{
  return mImpl->documents().get_map_document(id);
}

auto DocumentModel::get_tileset_document(const UUID& id) const -> const TilesetDocument&
{
  return mImpl->documents().get_tileset_document(id);
}

void DocumentModel::register_map(Shared<MapDocument> document)
{
  mImpl->documents().add_map_document(std::move(document));
}

void DocumentModel::register_tileset(Shared<TilesetDocument> document)
{
  mImpl->documents().add_tileset_document(std::move(document));
}

auto DocumentModel::unregister_map(const UUID& id) -> Shared<MapDocument>
{
  return mImpl->documents().remove_map_document(id);
}

auto DocumentModel::unregister_tileset(const UUID& id) -> Shared<TilesetDocument>
{
  return mImpl->documents().remove_tileset_document(id);
}

}  // namespace tactile
