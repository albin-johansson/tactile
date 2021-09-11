#pragma once

namespace Tactile {

/// \addtogroup commands
/// \{

struct CommandId final
{
  enum Value
  {
    AddRow,
    AddColumn,
    RemoveRow,
    RemoveColumn,

    AddLayer,
    RemoveLayer,
    DuplicateLayer,
    MoveLayerUp,
    MoveLayerDown,
    SetLayerOpacity,
    SetLayerVisible,
    RenameLayer,

    AddProperty,
    RemoveProperty,
    RenameProperty,
    UpdateProperty,
    ChangePropertyType,

    StampSequence,
    EraserSequence,
    Bucket,

    AddTileset,
    RemoveTileset
  };
};

/// \} End of group commands

}  // namespace Tactile
