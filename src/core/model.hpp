#pragma once

#include <entt.hpp>             // registry
#include <filesystem>           // path
#include <rune/everything.hpp>  // vector_map

#include "common/map_id.hpp"
#include "common/maybe.hpp"
#include "common/unique.hpp"
#include "document.hpp"
#include "events/command_events.hpp"
#include "events/tool_events.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class Model final
{
 public:
  // Store documents on the heap to ensure stability, which is important for commands
  using document_map = rune::vector_map<MapID, Unique<Document>>;
  using const_iterator = document_map::const_iterator;

  void Update();

  void OnCommandCapacityChanged(const SetCommandCapacityEvent& event);

  auto AddMap(Document document) -> MapID;

  auto AddMap(int tileWidth, int tileHeight) -> MapID;

  void SelectMap(MapID id);

  void RemoveMap(MapID id);

  [[nodiscard]] auto GetPath(MapID id) const -> const std::filesystem::path&;

  [[nodiscard]] auto HasDocumentWithPath(const std::filesystem::path& path) const -> bool;

  [[nodiscard]] auto GetActiveMapId() const -> Maybe<MapID>
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
  Maybe<MapID> mActiveMap;
  MapID mNextId{1};
};

/// \} End of group core

}  // namespace Tactile
