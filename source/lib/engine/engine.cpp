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
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "common/debug/assert.hpp"
#include "common/debug/logging.hpp"
#include "common/debug/stacktrace.hpp"
#include "common/fmt/stacktrace_formatter.hpp"
#include "engine/backend/gl/gl_backend.hpp"
#include "engine/backend/null/null_backend.hpp"
#include "engine/platform/window.hpp"
#include "io/directories.hpp"
#include "model/context.hpp"
#include "model/settings.hpp"
#include "model/systems/model_system.hpp"
#include "ui/style/fonts.hpp"

namespace tactile {

void on_terminate()
{
  try {
    spdlog::critical("Into exile I must go. Failed I have.\n{}",
                     boost::stacktrace::stacktrace {});
  }
  catch (...) {
    // Not much we can do in this case.
  }

  std::abort();
}

Engine::Engine(const BackendAPI api)
{
  std::set_terminate(&on_terminate);
  init_logger();

  mProtobuf.emplace();
  auto& sdl = mSDL.emplace(api);

  auto& window = sdl.get_window();
  win32_use_immersive_dark_mode(window);
  load_window_icon(window);

  mImGui.emplace();

  if (api == BackendAPI::Null) {
    spdlog::debug("[Engine] Using null backend");

    mBackend = std::make_unique<NullBackend>();
  }
  else if (api == BackendAPI::OpenGL) {
    spdlog::debug("[Engine] Initializing OpenGL backend");

    auto& gl_context = mSDL->get_gl_context();
    mBackend = std::make_unique<OpenGLBackend>(window.get(), gl_context.get());
  }

  spdlog::debug("[IO] Persistent file directory: {}", get_persistent_file_dir());

  sys::init_model(get_global_model());
}

void Engine::start()
{
  TACTILE_ASSERT(mBackend != nullptr);
  TACTILE_ASSERT(mApp != nullptr);

  spdlog::debug("[Engine] Starting core event loop");

  mRunning = true;

  mApp->on_startup();
  mSDL->get_window().show();
  mSDL->get_window().maximize();

  while (mRunning) {
    _poll_events();

    if (mApp->want_font_reload()) {
      _reload_fonts();
    }

    if (mBackend->new_frame().succeeded()) {
      mApp->on_update();
      mBackend->end_frame();
    }
  }

  mApp->on_shutdown();
  mSDL->get_window().hide();
}

void Engine::_poll_events()
{
  mKeyboard.refresh();

  cen::event_handler event;
  while (event.poll()) {
    mBackend->process_event(event.data());

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

void Engine::_reload_fonts()
{
  const auto& model = get_global_model();
  const auto& settings = model.get<Settings>();

  const auto font_size = settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT)
                             ? 13.0f
                             : static_cast<float>(settings.get_font_size());
  const auto use_default_font = settings.test_flag(SETTINGS_USE_DEFAULT_FONT_BIT);

  ui::reload_imgui_fonts(font_size, use_default_font);

  if (mBackend->reload_font_resources().succeeded()) {
    ImGui::GetStyle().ScaleAllSizes(1.0f);
    mApp->on_font_reload();
  }
}

void Engine::stop()
{
  mRunning = false;
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
