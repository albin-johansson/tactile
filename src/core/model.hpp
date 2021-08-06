#pragma once

#include <vector_map.hpp>  // vector_map

#include "aliases/map_id.hpp"
#include "aliases/maybe.hpp"
#include "aliases/unique.hpp"
#include "core/map_document.hpp"
#include "core/tools/mouse_tool_model.hpp"
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
  using document_storage = rune::vector_map<map_id, Unique<MapDocument>>;
  using const_iterator = document_storage::const_iterator;

  Model();

  void OnCommandCapacityChanged(const ChangeCommandCapacityEvent& event);

  /// \name Map document API
  /// \{

  auto AddMap(Unique<MapDocument> document) -> map_id;

  auto AddMap(int tileWidth, int tileHeight) -> map_id;

  void SelectMap(map_id id);

  void RemoveMap(map_id id);

  [[nodiscard]] auto GetDocument(map_id id) -> MapDocument*;

  [[nodiscard]] auto GetDocument(map_id id) const -> const MapDocument*;

  [[nodiscard]] auto GetActiveDocument() -> MapDocument*;

  [[nodiscard]] auto GetActiveDocument() const -> const MapDocument*;

  [[nodiscard]] auto GetActiveMap() -> Map&;

  [[nodiscard]] auto GetActiveMap() const -> const Map&;

  [[nodiscard]] auto GetActiveMapId() const -> Maybe<map_id>
  {
    return mActiveMap;
  }

  [[nodiscard]] auto begin() const noexcept -> const_iterator
  {
    return mDocuments.begin();
  }

  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return mDocuments.end();
  }

  /// \} End of map document API

  /// \name Tools API
  /// \{

  void SelectTool(MouseToolType tool);

  void OnMousePressed(const MousePressedEvent& event);

  void OnMouseReleased(const MouseReleasedEvent& event);

  void OnMouseDragged(const MouseDragEvent& event);

  [[nodiscard]] auto GetActiveTool() const -> MouseToolType;

  [[nodiscard]] auto IsStampActive() const -> bool;

  [[nodiscard]] auto IsEraserActive() const -> bool;

  [[nodiscard]] auto IsBucketActive() const -> bool;

  /// \} End of tools API

 private:
  document_storage mDocuments;
  MouseToolModel mTools;
  Maybe<map_id> mActiveMap;
  map_id mNextId{1};
};

/// \} End of group core

}  // namespace Tactile
