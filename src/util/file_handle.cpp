#include "file_handle.hpp"

namespace tactile {

file_handle::file_handle(const QString& path) : m_file{path}
{}

file_handle::~file_handle()
{
  m_file.close();
}

auto file_handle::open(const QFile::OpenMode mode) -> bool
{
  return m_file.open(mode);
}

auto file_handle::read() -> QByteArray
{
  return m_file.readAll();
}

auto file_handle::file() -> QFile&
{
  return m_file;
}

auto file_handle::file() const -> const QFile&
{
  return m_file;
}

}  // namespace tactile
