#pragma once

namespace Tactile {

/// \addtogroup core
/// \{

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
    SelectLayer,
    DuplicateLayer,
    MoveLayerUp,
    MoveLayerDown,
    SetLayerOpacity,
    SetLayerVisible,
    AddProperty,
    RemoveProperty,
    RenameProperty,
    SetProperty,
    ChangePropertyType,
    SetPropertyContext,
    StampSequence,
    EraserSequence,
    Bucket,
    AddTileset,
    RemoveTileset
  };
};

/// \} End of group commands

/// \} End of group core

}  // namespace Tactile
