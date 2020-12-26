#pragma once

#include <QStandardItemModel>

#include "map_document.hpp"

namespace tactile::vm {

class layer_item;

class layer_model final : public QStandardItemModel
{
  Q_OBJECT

 public:
  explicit layer_model(core::map_document* document);

  void add_tile_layer();

  void add_object_layer();

  void select(const QModelIndex& index);

  void remove(const QModelIndex& index);

  void duplicate(const QModelIndex& index);

  auto move_up(const QModelIndex& index) -> QModelIndex;

  auto move_down(const QModelIndex& index) -> QModelIndex;

  void set_opacity(const QModelIndex& index, double opacity);

  void set_visible(const QModelIndex& index, bool visible);

  [[nodiscard]] auto opacity(const QModelIndex& index) const -> double;

  [[nodiscard]] auto visible(const QModelIndex& index) const -> bool;

 private:
  core::map_document* m_document{};

  void add_item(layer_id id, const core::layer& layer);

  void remove_item(layer_id id);

  [[nodiscard]] auto get_item(const QModelIndex& index) -> layer_item*;

  [[nodiscard]] auto get_item(const QModelIndex& index) const
      -> const layer_item*;

  [[nodiscard]] auto id_from_index(const QModelIndex& index) const -> layer_id;
};

}  // namespace tactile::vm
