// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/runtime/runtime.hpp"

#include <cstdlib>        // malloc, free, EXIT_SUCCESS, EXIT_FAILURE
#include <exception>      // exception, set_terminate
#include <optional>       // optional
#include <unordered_map>  // unordered_map
#include <utility>        // move

#include <imgui.h>

#include "tactile/base/render/renderer.hpp"
#include "tactile/core/debug/terminate.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"
#include "tactile/core/log/terminal_log_sink.hpp"
#include "tactile/core/numeric/random.hpp"
#include "tactile/core/platform/win32.hpp"
#include "tactile/core/ui/common/style.hpp"
#include "tactile/runtime/protobuf_context.hpp"
#include "tactile/runtime/sdl_context.hpp"
#include "tactile/runtime/window.hpp"

namespace tactile {
namespace {

[[nodiscard]]
auto _imgui_malloc(const std::size_t bytes, void*) -> void*
{
  return std::malloc(bytes);
}

void _imgui_free(void* memory, void*)
{
  std::free(memory);
}

[[nodiscard]]
auto _make_logger() -> Logger
{
  win32_enable_virtual_terminal_processing();

  auto terminal_sink = std::make_unique<TerminalLogSink>();
  terminal_sink->use_ansi_colors(true);

  Logger logger {};

  logger.set_reference_instant(SteadyClock::now());
  logger.set_min_level(LogLevel::kTrace);
  logger.add_sink(std::move(terminal_sink));

  return logger;
}

}  // namespace

struct Runtime::Data final
{
  ProtobufContext protobuf_context;
  SDLContext sdl_context;
  Logger logger;
  std::optional<Window> window {};
  IRenderer* renderer {};
  std::unordered_map<CompressionFormat, ICompressor*> compression_formats {};
  std::unordered_map<SaveFormatId, ISaveFormat*> save_formats {};

  Data()
    : logger {_make_logger()}
  {
    set_default_logger(&logger);
  }

  ~Data() noexcept
  {
    TACTILE_LOG_TRACE("Destroying runtime data");
    set_default_logger(nullptr);
  }

  TACTILE_DELETE_COPY(Data);
  TACTILE_DELETE_MOVE(Data);
};

Runtime::Runtime()
{
  std::set_terminate(&on_terminate);
  mData = std::make_unique<Data>();

  init_random_number_generator();
}

Runtime::~Runtime() noexcept = default;

void Runtime::init_window(const std::uint32_t flags)
{
  if (auto window = Window::create(flags)) {
    win32_use_immersive_dark_mode(window->get_handle());
    mData->window = std::move(*window);
  }
}

void Runtime::set_renderer(IRenderer* renderer)
{
  mData->renderer = renderer;
}

void Runtime::set_compression_format(const CompressionFormat id, ICompressor* format)
{
  if (format != nullptr) {
    mData->compression_formats.insert_or_assign(id, format);
  }
  else {
    mData->compression_formats.erase(id);
  }
}

void Runtime::set_save_format(const SaveFormatId id, ISaveFormat* format)
{
  if (format != nullptr) {
    mData->save_formats.insert_or_assign(id, format);
  }
  else {
    mData->save_formats.erase(id);
  }
}

auto Runtime::get_window() -> IWindow*
{
  return mData->window.has_value() ? &mData->window.value() : nullptr;
}

auto Runtime::get_renderer() -> IRenderer*
{
  return mData->renderer;
}

auto Runtime::get_compression_format(const CompressionFormat id) const -> const ICompressor*
{
  const auto iter = mData->compression_formats.find(id);

  if (iter != mData->compression_formats.end()) {
    return iter->second;
  }

  return nullptr;
}

auto Runtime::get_save_format(const SaveFormatId id) const -> const ISaveFormat*
{
  const auto iter = mData->save_formats.find(id);

  if (iter != mData->save_formats.end()) {
    return iter->second;
  }

  return nullptr;
}

void Runtime::get_imgui_allocator_functions(imgui_malloc_fn** malloc_fn,
                                            imgui_free_fn** free_fn,
                                            void** user_data)
{
  if (malloc_fn) {
    *malloc_fn = &_imgui_malloc;
  }

  if (free_fn) {
    *free_fn = &_imgui_free;
  }

  if (user_data) {
    *user_data = nullptr;
  }
}

}  // namespace tactile
