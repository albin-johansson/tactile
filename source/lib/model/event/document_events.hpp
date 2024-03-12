// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Event emitted when a loaded document should be opened in its own tab.
struct OpenDocumentEvent final {
  UUID document_id {};
};

struct CloseDocumentEvent final {
  UUID document_id {};
};

struct SelectDocumentEvent final {
  UUID document_id {};
};

struct SaveEvent final {};

struct SaveAsEvent final {
  Path path;
};

struct OpenSaveAsDialogEvent final {};

}  // namespace tactile
