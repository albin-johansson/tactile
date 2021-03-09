#pragma once

#include <QFileInfo>           // QFileInfo
#include <QStandardItemModel>  // QStandardItemModel

#include "forward_declare.hpp"
#include "not_null.hpp"
#include "tile_height.hpp"
#include "tile_width.hpp"
#include "tileset_selection.hpp"

TACTILE_FORWARD_DECLARE(tactile::core, map_document)

namespace tactile::vm {

class tileset_model final : public QStandardItemModel
{
  Q_OBJECT

 public:
  explicit tileset_model(not_null<core::map_document*> document,
                         QObject* parent = nullptr);

  void add(const QImage& image,
           const QFileInfo& path,
           const QString& name,
           tile_width tileWidth,
           tile_height tileHeight);

  void remove(const QModelIndex& index);

  void select(const QModelIndex& index);

  void rename(const QModelIndex& index);

  void set_selection(const core::tileset_selection& selection);

 private:
  core::map_document* m_document{};
};

}  // namespace tactile::vm
