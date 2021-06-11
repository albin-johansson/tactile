#include "file_handle.hpp"

namespace tactile {

FileHandle::FileHandle(const QString& path) : mFile{path}
{}

FileHandle::~FileHandle()
{
  mFile.close();
}

auto FileHandle::Open(const QFile::OpenMode mode) -> bool
{
  return mFile.open(mode);
}

auto FileHandle::Read() -> QByteArray
{
  return mFile.readAll();
}

auto FileHandle::File() -> QFile&
{
  return mFile;
}

auto FileHandle::File() const -> const QFile&
{
  return mFile;
}

auto FileHandle::Exists() const -> bool
{
  return mFile.exists();
}

}  // namespace tactile
