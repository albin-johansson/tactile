#pragma once

#include <QStandardItemModel>  // QStandardItemModel

#include "forward_declare.hpp"
#include "layer_id.hpp"
#include "maybe.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)
TACTILE_FORWARD_DECLARE(tactile::core, ILayer)

namespace tactile::vm {

class layer_item;

class layer_model final : public QStandardItemModel
{
  Q_OBJECT

 public:
  explicit layer_model(not_null<core::map_document*> document);

  void add_tile_layer();

  void add_object_layer();

  void show_properties(const QModelIndex& index);

  void select(const QModelIndex& index);

  void remove(const QModelIndex& index);

  void duplicate(const QModelIndex& index);

  void move_up(const QModelIndex& index);

  void move_down(const QModelIndex& index);

  void set_name(const QModelIndex& index, const QString& name);

  void set_opacity(const QModelIndex& index, double opacity);

  void set_visible(const QModelIndex& index, bool visible);

  [[nodiscard]] auto opacity(const QModelIndex& index) const -> double;

  [[nodiscard]] auto visible(const QModelIndex& index) const -> bool;

  [[nodiscard]] auto index_of(layer_id id) const -> maybe<QModelIndex>;

 signals:
  void changed_name(layer_id id, const QString& name);
  void changed_opacity(layer_id id, double opacity);
  void changed_visibility(layer_id id, bool visible);
  void selected_layer(layer_id id, const core::ILayer& layer);

 private:
  core::map_document* m_document{};
  maybe<int> m_duplicateTargetRow;

  void add_item(layer_id id, const core::ILayer& layer);

  void remove_item(layer_id id);

  auto move_up_in_gui(layer_id id) -> maybe<QModelIndex>;

  auto move_down_in_gui(layer_id id) -> maybe<QModelIndex>;

  [[nodiscard]] auto get_item(const QModelIndex& index) const
      -> const layer_item*;

  [[nodiscard]] auto id_from_index(const QModelIndex& index) const -> layer_id;

 private slots:
  void moved_layer_forward(layer_id id);

  void moved_layer_back(layer_id id);
};

}  // namespace tactile::vm
