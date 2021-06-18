#pragma once

#include <QWidget>  // QWidget

#include "core_fwd.hpp"
#include "gui_fwd.hpp"
#include "map_id.hpp"
#include "smart_pointers.hpp"
#include "tactile_declare_ui.hpp"
#include "tileset_id.hpp"

TACTILE_DECLARE_UI(TilesetWidget)

namespace tactile {

class TilesetWidget final : public QWidget
{
  Q_OBJECT

 public:
  explicit TilesetWidget(QWidget* parent = nullptr);

  ~TilesetWidget() noexcept override;

 signals:
  void S_AddTileset();
  void S_RemoveTileset(tileset_id id);
  void S_SelectTileset(tileset_id id);
  void S_RenameTileset(tileset_id id, const QString& name);
  void S_ShowProperties(tileset_id id);
  void S_SetTilesetSelection(const core::TilesetSelection& selection);

 public slots:
  void OnAddedTileset(map_id map, tileset_id id, const core::Tileset& tileset);

  void OnRemovedTileset(tileset_id id);

  void OnRenamedTileset(tileset_id id, const QString& name);

  void OnSwitchedMap(map_id id);

 private:
  Unique<Ui::TilesetWidget> mUi;
  TilesetContentPage* mContentPage{};
  TilesetEmptyPage* mEmptyPage{};
  int mEmptyIndex{};
  int mContentIndex{};

 private slots:
  void OnSwitchToEmptyPage();
  void OnSwitchToContentPage();
};

}  // namespace tactile
