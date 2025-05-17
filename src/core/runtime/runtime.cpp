// Copyright (C) 2025 Albin Johansson

module tactile.core.runtime;

namespace tactile {

void Runtime::set_save_encoder(const SaveFormatKind kind, ISaveEncoder* encoder)
{
  m_save_encoders.insert_or_assign(kind, encoder);
}

void Runtime::set_save_decoder(const SaveFormatKind kind, ISaveDecoder* decoder)
{
  m_save_decoders.insert_or_assign(kind, decoder);
}

auto Runtime::find_save_encoder(const SaveFormatKind kind) -> ISaveEncoder*
{
  const auto iter = m_save_encoders.find(kind);
  return iter != m_save_encoders.end() ? iter->second : nullptr;
}

auto Runtime::find_save_encoder(const SaveFormatKind kind) const
    -> const ISaveEncoder*
{
  const auto iter = m_save_encoders.find(kind);
  return iter != m_save_encoders.end() ? iter->second : nullptr;
}

auto Runtime::find_save_decoder(const SaveFormatKind kind) -> ISaveDecoder*
{
  const auto iter = m_save_decoders.find(kind);
  return iter != m_save_decoders.end() ? iter->second : nullptr;
}

auto Runtime::find_save_decoder(const SaveFormatKind kind) const
    -> const ISaveDecoder*
{
  const auto iter = m_save_decoders.find(kind);
  return iter != m_save_decoders.end() ? iter->second : nullptr;
}

auto Runtime::compressor() -> Compressor&
{
  return m_compressor;
}

auto Runtime::compressor() const -> const Compressor&
{
  return m_compressor;
}

auto get_runtime() -> Runtime&
{
  static Runtime runtime {};
  return runtime;
}

}  // namespace tactile
