#pragma once

#include <QIcon>  // QIcon

namespace tactile {

[[nodiscard]] auto TactileLogo() -> const QIcon&;

[[nodiscard]] auto IconAdd() -> const QIcon&;

[[nodiscard]] auto IconRemove() -> const QIcon&;

[[nodiscard]] auto IconRename() -> const QIcon&;

[[nodiscard]] auto IconDuplicate() -> const QIcon&;

[[nodiscard]] auto IconCopy() -> const QIcon&;

[[nodiscard]] auto IconOpen() -> const QIcon&;

[[nodiscard]] auto IconSaveAs() -> const QIcon&;

[[nodiscard]] auto IconReset() -> const QIcon&;

[[nodiscard]] auto IconPaste() -> const QIcon&;

[[nodiscard]] auto IconTileLayer() -> const QIcon&;

[[nodiscard]] auto IconObjectLayer() -> const QIcon&;

[[nodiscard]] auto IconVisible() -> const QIcon&;

[[nodiscard]] auto IconInvisible() -> const QIcon&;

[[nodiscard]] auto IconMoveUp() -> const QIcon&;

[[nodiscard]] auto IconMoveDown() -> const QIcon&;

[[nodiscard]] auto IconCollapsed() -> const QIcon&;

[[nodiscard]] auto IconExpanded() -> const QIcon&;

[[nodiscard]] auto IconQuestionMark() -> const QIcon&;

}  // namespace tactile
