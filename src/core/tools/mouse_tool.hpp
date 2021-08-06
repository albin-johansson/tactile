#pragma once

#include "aliases/not_null.hpp"
#include "mouse_info.hpp"
#include "mouse_tool_type.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class Model;
class MapDocument;
class Tileset;

class AMouseTool
{
 public:
  explicit AMouseTool(NotNull<Model*> model);

  virtual ~AMouseTool() noexcept = default;

  virtual void Enable()
  {}

  virtual void Disable()
  {}

  virtual void OnPressed(const MouseInfo& info)
  {}

  virtual void OnDragged(const MouseInfo& info)
  {}

  virtual void OnReleased(const MouseInfo& info)
  {}

  [[nodiscard]] virtual auto GetType() const -> MouseToolType = 0;

 protected:
  [[nodiscard]] auto GetModel() -> Model&
  {
    return *mModel;
  }

  [[nodiscard]] auto GetDocument() -> MapDocument*;

  [[nodiscard]] auto GetDocument() const -> const MapDocument*;

  [[nodiscard]] auto GetTileset() const -> const Tileset*;

 private:
  Model* mModel{};
};

/// \} End of group core

}  // namespace Tactile
