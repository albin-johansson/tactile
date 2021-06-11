#pragma once

#include <QWidget>  // QWidget
#include <map>      // map

#include "forward_declare.hpp"
#include "map_id.hpp"
#include "maybe.hpp"
#include "position.hpp"
#include "smart_pointers.hpp"
#include "tileset.hpp"
#include "tileset_tab_manager.hpp"

TACTILE_FORWARD_DECLARE_UI(TilesetContentPage)

TACTILE_FORWARD_DECLARE(tactile, TabWidget)
TACTILE_FORWARD_DECLARE(tactile, TilesetTab)
TACTILE_FORWARD_DECLARE(tactile, TilesetTabContextMenu)

namespace tactile {

/**
 * \class TilesetContentPage
 *
 * \brief Represents the page with the tilesets associated with a map.
 *
 * \note This widget is meant to be shown when a map has at least one available
 * tileset.
 *
 * \since 0.1.0
 *
 * \headerfile tileset_content_page.hpp
 */
class TilesetContentPage final : public QWidget
{
  Q_OBJECT

 public:
  explicit TilesetContentPage(QWidget* parent = nullptr);

  ~TilesetContentPage() noexcept override;

  /**
   * \brief Indicates whether or not there are any tileset tabs.
   *
   * \return `true` if there are no tileset tabs; `false` otherwise.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto IsEmpty() const -> bool;

 signals:
  void S_SwitchToEmptyPage();
  void S_SwitchToContentPage();
  void S_AddTileset();
  void S_SelectTileset(tileset_id id);
  void S_RemoveTileset(tileset_id id);
  void S_RenameTileset(tileset_id id, const QString& name);
  void S_SetTilesetSelection(const core::TilesetSelection& selection);

 public slots:
  /**
   * \brief Makes the tileset content page display the tilesets associated with
   * the specified map.
   *
   * \param map the ID associated with the selected map.
   *
   * \since 0.1.0
   */
  void OnSelectedMap(map_id map);

  /**
   * \brief Adds a tileset tab that represents the supplied tileset.
   *
   * \pre `Id` must not have been added before.
   *
   * \param map the ID associated with the map that owns the tileset.
   * \param id the ID associated with the tileset that will be added.
   * \param tileset the tileset that will be added.
   *
   * \since 0.1.0
   */
  void OnAddedTileset(map_id map, tileset_id id, const core::Tileset& tileset);

  /**
   * \brief Removes the tileset tab associated with the specified tileset.
   *
   * \pre `Id` must be associated with an existing tileset tab.
   *
   * \param id the ID associated with the tileset tab which will be removed.
   *
   * \since 0.1.0
   */
  void OnRemovedTileset(tileset_id id);

  void OnRenamedTileset(tileset_id id, const QString& name);

 private:
  unique<Ui::TilesetContentPage> mUi;
  TabWidget* mTabWidget{};
  TilesetTabContextMenu* mContextMenu{};
  maybe<map_id> mCurrentMap;
  std::map<map_id, TilesetTabManager> mTabManagers;

  /**
   * \brief Switches to the tileset tabs associated with the specified map.
   *
   * \param map the ID of the map to switch to.
   *
   * \since 0.1.0
   *
   * \signal `S_SelectTileset`
   */
  void SwitchTo(map_id map);

  /**
   * \brief Adds the corner button to the tab widget, which is used to Add
   * tilesets.
   *
   * \since 0.1.0
   */
  void AddCornerButton();

  void TriggerContextMenu(const QPoint& pos);

  /**
   * \brief Returns the tab associated with the specified index.
   *
   * \param index the index of the desired tab.
   *
   * \return a pointer to the found tab; `nullptr` if no tab was found.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto TabFromIndex(int index) -> TilesetTab*;

  /**
   * \brief Returns the tab manager associated with the current map.
   *
   * \note This function throws if there is no current map *or* if there is no
   * manager associated with the current map.
   *
   * \return the tab manager associated with the current map.
   *
   * \since 0.1.0
   */
  [[nodiscard]] auto CurrentManager() -> TilesetTabManager&;
};

}  // namespace tactile
