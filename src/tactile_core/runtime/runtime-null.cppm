// Copyright (C) 2025 Albin Johansson

export module tactile.core.runtime:null;

export import :interfaces;

export namespace tactile {

/// A null backend implementation.
class NullBackend final : public IBackend
{
 public:
  [[nodiscard]]
  auto run(IApp&) -> ExitCode override
  {
    return ExitCode::kSuccess;
  }
};

}  // namespace tactile
