// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(DocumentModel)

/// Restores the open documents from the last session.
void load_session_from_disk(DocumentModel& model);

/// Saves the current session, i.e. the paths of the currently open documents.
void save_session_to_disk(const DocumentModel& model);

}  // namespace tactile
