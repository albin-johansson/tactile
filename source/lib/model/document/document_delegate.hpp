// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/component/component_index.hpp"
#include "core/context/context_manager.hpp"
#include "core/viewport.hpp"
#include "model/cmd/command_stack.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

class DocumentDelegate final {
 public:
  explicit DocumentDelegate(const UUID& root_context);

  void set_component_index(Shared<ComponentIndex> index);

  void set_path(Path path);

  [[nodiscard]] auto get_component_index() -> const Shared<ComponentIndex>&;
  [[nodiscard]] auto view_component_index() const -> const ComponentIndex*;

  [[nodiscard]] auto path() const -> const Path&;

  [[nodiscard]] auto has_path() const -> bool;

  [[nodiscard]] auto get_contexts() -> ContextManager&;
  [[nodiscard]] auto get_contexts() const -> const ContextManager&;

  [[nodiscard]] auto get_history() -> CommandStack&;
  [[nodiscard]] auto get_history() const -> const CommandStack&;

  [[nodiscard]] auto get_viewport() -> Viewport&;
  [[nodiscard]] auto get_viewport() const -> const Viewport&;

 private:
  ContextManager mContexts;
  CommandStack mCommands;
  Shared<ComponentIndex> mComponentIndex;
  Viewport mViewport;
  Maybe<Path> mPath;
};

}  // namespace tactile
