/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "engine.hpp"

#include <cstdlib>    // abort
#include <exception>  // set_terminate
#include <utility>    // move

#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "core/debug/assert.hpp"
#include "core/debug/stacktrace.hpp"
#include "core/formatters/stacktrace_formatter.hpp"
#include "engine/app_delegate.hpp"
#include "engine/platform/window.hpp"
#include "io/directories.hpp"
#include "model/services/backend_service.hpp"
#include "model/services/backends/gl_backend_service.hpp"
#include "model/services/backends/null_backend_service.hpp"
#include "model/services/logging_service.hpp"
#include "model/services/service_locator.hpp"

namespace tactile {


Engine::Engine(const BackendAPI api)
    : mAPI {api}
{
  std::set_terminate(&on_terminate);

  mLoggingService = std::make_unique<LoggingService>();
  mLoggingService->install_logger();

  ServiceLocator<LoggingService>::reset(mLoggingService.get());

  mProtobuf.emplace();
  auto& sdl = mSDL.emplace(mAPI);

  auto& window = sdl.get_window();
  win32_use_immersive_dark_mode(window);
  load_window_icon(window);

  mImGui.emplace();

  if (mAPI == BackendAPI::Null) {
    spdlog::debug("[Engine] Using null backend");
    mBackendService = std::make_unique<NullBackendService>();
  }
  else if (mAPI == BackendAPI::OpenGL) {
    spdlog::debug("[Engine] Initializing OpenGL backend");

    auto& gl_context = mSDL->get_gl_context();
    mBackendService = std::make_unique<GLBackendService>(window.get(), gl_context.get());
  }

  ServiceLocator<BackendService>::reset(mBackendService.get());

  spdlog::debug("[IO] Persistent file directory: {}", get_persistent_file_dir());
}

Engine::~Engine() noexcept = default;

void Engine::start()
{
  TACTILE_ASSERT(mBackendService != nullptr);
  TACTILE_ASSERT(mApp != nullptr);

  spdlog::debug("[Engine] Starting core event loop");

  mRunning = true;

  mApp->on_startup(mAPI);
  mSDL->get_window().show();
  mSDL->get_window().maximize();

  while (mRunning) {
    mRunning = !mApp->should_stop();
    _poll_events();
    mApp->on_update();
  }

  mApp->on_shutdown();
  mSDL->get_window().hide();
}

void Engine::_poll_events()
{
  mKeyboard.refresh();

  cen::event_handler event;
  while (event.poll()) {
    mBackendService->process_event(*event.data());

    switch (event.type().value()) {
      case cen::event_type::quit:
        mRunning = false;
        break;

      default:
        mApp->on_event(event);
        break;
    }
  }
}

void Engine::set_app_delegate(Unique<AppDelegate> app)
{
  mApp = std::move(app);
}

auto Engine::get_window() -> cen::window&
{
  return mSDL.value().get_window();
}

}  // namespace tactile
