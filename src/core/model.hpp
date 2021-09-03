#pragma once

#include <entt.hpp>             // registry
#include <rune/everything.hpp>  // vector_map

#include "aliases/map_id.hpp"
#include "aliases/maybe.hpp"
#include "aliases/unique.hpp"
#include "document.hpp"
#include "events/change_command_capacity_event.hpp"
#include "events/tools/mouse_drag_event.hpp"
#include "events/tools/mouse_pressed_event.hpp"
#include "events/tools/mouse_released_event.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class Model final
{
 public:
  using document_map = rune::vector_map<map_id, Document>;
  using const_iterator = document_map::const_iterator;

  void Update();

  void OnCommandCapacityChanged(const ChangeCommandCapacityEvent& event);

  auto AddMap(Document document) -> map_id;

  auto AddMap(int tileWidth, int tileHeight) -> map_id;

  void SelectMap(map_id id);

  void RemoveMap(map_id id);

  [[nodiscard]] auto GetActiveMapId() const -> Maybe<map_id>
  {
    return mActiveMap;
  }

  [[nodiscard]] auto HasActiveDocument() const -> bool;

  [[nodiscard]] auto GetActiveDocument() -> Document*;

  [[nodiscard]] auto GetActiveDocument() const -> const Document*;

  [[nodiscard]] auto GetActiveRegistry() -> entt::registry*;

  [[nodiscard]] auto GetActiveRegistry() const -> const entt::registry*;

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return mDocuments.begin();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return mDocuments.end();
  }

  //  void SelectTool(MouseToolType tool);

  void OnMousePressed(const MousePressedEvent& event);

  void OnMouseReleased(const MouseReleasedEvent& event);

  void OnMouseDragged(const MouseDragEvent& event);


  [[nodiscard]] auto IsStampActive() const -> bool;

  [[nodiscard]] auto IsEraserActive() const -> bool;

  [[nodiscard]] auto IsBucketActive() const -> bool;

  [[nodiscard]] auto IsClean() const -> bool;

  [[nodiscard]] auto CanUndo() const -> bool;

  [[nodiscard]] auto CanRedo() const -> bool;

  [[nodiscard]] auto GetUndoText() const -> const std::string&;

  [[nodiscard]] auto GetRedoText() const -> const std::string&;

  [[nodiscard]] auto CanSaveDocument() const -> bool;

  [[nodiscard]] auto CanDecreaseViewportTileSize() const -> bool;

 private:
  document_map mDocuments;
  Maybe<map_id> mActiveMap;
  map_id mNextId{1};
};

/// \} End of group core

}  // namespace Tactile
